#include <array>
#include <bits/stdc++.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace std;

void classifyRecursively(string);

void classifyCurrentDirectory(string _rootDirectory);
void classifyRecursivelyKeep(string _rootDirectory);
void moveFilesToRoot(string _rootDirectory, string _currentDirectory);

vector<string> getFilesFromDirectory(string _rootDirectory);
vector<string> getSubdirectories(string _rootDirectory);
string getFileCategory(string _fileName);

std::string exec(string _cmd)
{
    //converting _cmd to char array
    char *temp = new char[_cmd.size() + 1];
    strcpy(temp, _cmd.c_str());

    std::array<char, 128> buffer;
    std::string result;
    /* 
    unique_ptr is a smart pointer that has two components:
        1-a stored pointer: a pointer to the object it manages.
        2-a stored deleter: a callable object that takes an argument of the same type as the stored pointer and is called to delete the managed object
    */
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(temp, "r"), pclose);

    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

map<string, vector<string>> Data;

int main(int argc, char **argv)
{

    Data["music"].push_back(".mp3");
    Data["videos"].push_back(".mkv");
    Data["documents"].push_back(".pdf");

    if (argc == 1)
    {
        throw std::runtime_error("not enough arguments, use -help for usage info\n");
        return 1;
    }
    else if (argc == 2)
    {
        string arg1 = argv[1];
        if (arg1 == "-help")
        {
            ifstream inFile;
            inFile.open("../documentation.txt");
            string line;
            while (getline(inFile, line))
                cout << line << endl;
        }
        else
        {
            classifyCurrentDirectory(argv[1]);
        }
    }
    else
    {
        string option = argv[1];

        if (option == "-rk")
            classifyRecursivelyKeep(argv[2]);
        else if (option == "-rr")
        {
            moveFilesToRoot(argv[2], argv[2]);
            classifyCurrentDirectory(argv[2]);
        }
    }
    return 0;
}

/*
testing:
/home/undo/Development/Projects/FileClassifier/testingDir
/home/undo/Desktop
*/

vector<string> getFilesFromDirectory(string _rootDirectory)
{
    string command = "";
    // list the content of the current directory
    command += "ls ";
    command += _rootDirectory;

    // argument of ls, puts a '/' at the end of each subdirectory
    command += " -p ";

    // piping to look for lines not ending with "/" i.e: files not directories
    command += "| grep -v /";

    string commandOutput = exec(command);

    vector<string> files;
    while (commandOutput.find('\n') != string::npos)
    {
        string file = commandOutput.substr(0, commandOutput.find('\n'));
        files.push_back(file);
        commandOutput = commandOutput.substr(commandOutput.find('\n') + 1, commandOutput.size());
    }
    return files;
}

string getFileCategory(string _extension)
{
    for (auto i : Data)
    {
        string category = i.first;
        vector<string> extensions = i.second;

        for (auto j : extensions)
        {
            if (j == _extension)
                return category;
        }
    }

    return "unknown";
}

void classifyCurrentDirectory(string _rootDirectory)
{
    vector<string> files = getFilesFromDirectory(_rootDirectory);
    set<string> categories;

    for (auto file : files)
    {
        string extension = file.substr(file.find('.'), file.size());
        string category = getFileCategory(extension);

        if (categories.find(category) == categories.end())
        {
            string command = "";
            command += "mkdir ";
            command += _rootDirectory;
            command += "/";
            command += category;
            categories.insert(category);
            exec(command);
        }

        string command = "";
        command += "mv ";
        command += _rootDirectory;
        command += "/";
        command += file;
        command += " ";
        command += _rootDirectory;
        command += "/";
        command += category;
        exec(command);
    }
}

void classifyRecursivelyKeep(string _rootDirectory)
{
    cout << _rootDirectory << endl;
    vector<string> subDirectories = getSubdirectories(_rootDirectory);
    for (auto subDirectory : subDirectories)
    {
        classifyRecursivelyKeep(_rootDirectory + "/" + subDirectory);
    }
    classifyCurrentDirectory(_rootDirectory);
}

void moveFilesToRoot(string _rootDirectory, string _currentDirectory)
{
    vector<string> subDirectories = getSubdirectories(_currentDirectory);
    cout << _rootDirectory << ": " << _currentDirectory << endl;
    for (auto subDirectory : subDirectories)
    {
        moveFilesToRoot(_rootDirectory, _currentDirectory + "/" + subDirectory);
    }
    vector<string> files = getFilesFromDirectory(_currentDirectory);
    for (auto file : files)
    {
        if (_currentDirectory != _rootDirectory)
        {
            string command = "";
            command += "mv ";
            command += _currentDirectory;
            command += "/";
            command += file;
            command += " ";
            command += _rootDirectory;
            exec(command);
        }
    }
    if (_currentDirectory != _rootDirectory)
    {
        string command = "";
        command += "cd .. && rm -d ";
        command += _currentDirectory;
        exec(command);
    }
}

vector<string> getSubdirectories(string _rootDirectory)
{
    string command = "";
    // list the content of the current directory
    command += "ls ";
    command += _rootDirectory;

    // argument of ls, puts a '/' at the end of each subdirectory
    command += " -p ";

    // piping to look for lines not ending with "/" i.e: files not directories
    command += "| grep /";

    string commandOutput = exec(command);
    // cout << commandOutput << endl;

    vector<string> getSubdirectories;
    while (commandOutput.find('\n') != string::npos)
    {
        string subDirectory = commandOutput.substr(0, commandOutput.find('\n'));
        subDirectory = subDirectory.substr(0, subDirectory.size() - 1);
        getSubdirectories.push_back(subDirectory);
        commandOutput = commandOutput.substr(commandOutput.find('\n') + 1, commandOutput.size());
    }
    return getSubdirectories;
}
