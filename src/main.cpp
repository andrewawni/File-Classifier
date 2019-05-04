#include <array>
#include <bits/stdc++.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace std;

void classifyRecursively(string);

std::string exec(const char* cmd)
{

    std::array<char, 128> buffer;
    std::string result;
    /* 
    unique_ptr is a smart pointer that has two components:
        1-a stored pointer: a pointer to the object it manages.
        2-a stored deleter: a callable object that takes an argument of the same type as the stored pointer and is called to delete the managed object
    */

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main(int argc, char** argv)
{
    //example
    // string output = exec("ls");
    // cout << "Directory Content:\n"+ output;
    // cout << argc << endl;
    // for(int i = 0; i<argc; i++)
    // {
    //     cout << argv[i] << endl;./

    // }

    string option = argv[1];
    if (option == "-r") {
        classifyRecursively(argv[2]);
        cout << "here";
    }
    return 0;
}

/*
/home/undo/Development/Projects/FileClassifier/testingDir
*/

class Node {
public:
    int cntNodes = 0;
    string selfPath;
    string parentPath;
    Node* children[];
};

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
    char* temp = new char[cmd.size() + 1];
    strcpy(temp, cmd.c_str());
    string content = exec(temp);

    string tempContent = "";
    for (int i = 0; i < content.size(); i++) {
        if (content[i] != '\n')
            tempContent += content[i];
        else {
            string type = "";
            for (int j = 0; j < tempContent.size(); j++) {
                if (tempContent[j] == '.') {
                    for (int k = j + 1; k < tempContent.size(); k++) {
                        type += tempContent[k];
                    }

                } else {
                    continue;
                }
            }

            ret.push_back({ _rootDir + tempContent, TypeOfFile(type) });
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
    Node* root = new Node();

    root->selfPath = rootDir;
    root->parentPath = " ";

    string cmd = "";
    cmd += "ls ";
    cmd += rootDir;
    cmd += " -p ";
    cmd += "| grep /";

    char* temp = new char[cmd.size() + 1];
    strcpy(temp, cmd.c_str());

    string children = exec(temp);
    int lastIndex = 0;
    //cout<<children.size();

    string tmp = "";
    for (int i = 0; i < children.size(); i++) {
        if (children[i] != '/') {
            tmp += children[i];
        } else {
            Node* child = new Node();
            child->parentPath = rootDir;
            child->selfPath = child->parentPath + "/" + tmp;
            root->children[root->cntNodes++] = child;
            tmp = "";
            i += 1;
        }
    }
    for (int i = 0; i < root->cntNodes; i++) {
        // cout << root->children[i]->selfPath << endl;
        vector<pair<string, string>> tmp = getContent(root->children[i]->selfPath);
        for (int j = 0; j < tmp.size(); j++) {
            cout << tmp[j].first << " " << tmp[j].second << endl;
        }
        classifyRecursively(root->children[i]->selfPath);
    }

    for (int i = 0; i < 5; i++) {
        vector<int> v;
        v.push_back(5);
    }
}
