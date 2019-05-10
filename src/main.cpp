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
    //example
    // string output = exec("ls");
    // cout << "Directory Content:\n"+ output;
    // cout << argc << endl;
    // for(int i = 0; i<argc; i++)
    // {
    //     cout << argv[i] << endl;./

    // }

    // data["music"] = {".mp3", ".wav"};
    // data["videos"] = {".mp4", ".mkv"};
    // data["documents"] = {".pdf", ".docx"};
    // data["images"] = {".png", ".jpeg"};

    Data["music"].push_back(".mp3");
    Data["videos"].push_back(".mkv");
    Data["documents"].push_back(".pdf");

    string option = argv[1];
    cout << argc << endl;
    if (argc == 2)
    {
        //default
        classifyCurrentDirectory(argv[1]);
    }
    else
    {
    
        classifyRecursivelyKeep(argv[1]);
        
    }
    return 0;
}


/*
/home/undo/Development/Projects/FileClassifier/testingDir
/home/undo/Desktop
*/

class Node
{
public:
    int cntNodes = 0;
    string selfPath;
    string parentPath;
    Node *children[];
};

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
    vector<string> subDirectories = getSubdirectories(_rootDirectory);
    for (auto subDirectory : subDirectories)
    {
        classifyRecursivelyKeep(_rootDirectory + "/" + subDirectory);
    }
    classifyCurrentDirectory(_rootDirectory);
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
        getSubdirectories.push_back(subDirectory);
        commandOutput = commandOutput.substr(commandOutput.find('\n') + 1, commandOutput.size());
    }
    return getSubdirectories;
}

/*
string TypeOfFile(string s)
{
    if (s == "mp3")
        return "music";
    else if (s == "mp4")
        return "video";
    else if (s == "png" || s == "jpg")
        return "image";
    else if (s == "txt")
        return "document";
    else
        return "not known";
}
vector<pair<string, string>> getContent(string _rootDir)
{

    vector<pair<string, string>> ret;
    string cmd = "";
    cmd += "ls ";
    cmd += _rootDir;
    cmd += " -p ";
    cmd += "| grep -v /";
    char *temp = new char[cmd.size() + 1];
    strcpy(temp, cmd.c_str());
    string content = exec(temp);

    string tempContent = "";
    for (int i = 0; i < content.size(); i++)
    {
        if (content[i] != '\n')
            tempContent += content[i];
        else
        {
            string type = "";
            for (int j = 0; j < tempContent.size(); j++)
            {
                if (tempContent[j] == '.')
                {
                    for (int k = j + 1; k < tempContent.size(); k++)
                    {
                        type += tempContent[k];
                    }
                }
                else
                {
                    continue;
                }
            }

            ret.push_back({_rootDir + tempContent, TypeOfFile(type)});
            tempContent = "";
            i += 1;
        }
    }
    return ret;
}
void classifyRecursively(string _rootDir)
{
    string rootDir = _rootDir;
    // cout << rootDir << endl;
    Node *root = new Node();

    root->selfPath = rootDir;
    root->parentPath = " ";

    string cmd = "";
    cmd += "ls ";
    cmd += rootDir;
    cmd += " -p ";
    cmd += "| grep /";

    char *temp = new char[cmd.size() + 1];
    strcpy(temp, cmd.c_str());

    string children = exec(temp);
    int lastIndex = 0;
    //cout<<children.size();

    string tmp = "";
    for (int i = 0; i < children.size(); i++)
    {
        if (children[i] != '/')
        {
            tmp += children[i];
        }
        else
        {
            Node *child = new Node();
            child->parentPath = rootDir;
            child->selfPath = child->parentPath + "/" + tmp;
            root->children[root->cntNodes++] = child;
            tmp = "";
            i += 1;
        }
    }
    for (int i = 0; i < root->cntNodes; i++)
    {
        // cout << root->children[i]->selfPath << endl;
        vector<pair<string, string>> tmp = getContent(root->children[i]->selfPath);
        for (int j = 0; j < tmp.size(); j++)
        {
            cout << tmp[j].first << " " << tmp[j].second << endl;
        }
        classifyRecursively(root->children[i]->selfPath);
    }

    for (int i = 0; i < 5; i++)
    {
        vector<int> v;
        v.push_back(5);
    }
}
*/