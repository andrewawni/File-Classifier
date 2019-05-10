/*
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include "string"
#include <array>
#include "rapidxml.hpp"
#include <fstream>
#include <map>
#include <vector>
//#include <bits/stdc++.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include "Source.h"

using namespace std;
// it takes the command  as an argument and then give it to the system , the function 
// returns the output that done by the system as result  from the command .
std::string exec(const char* cmd)
{
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);

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
void mkdir( char* fileName);
map <string,vector<string> > Map ;
void c(string f , string e )
{ 		
	map<string, vector<string> >:: iterator it;
	for (      it = Map.begin() ; it!=Map.end(); it++){
		//cout << it->first << " : ";
		for (int i = 0 ; i < it->second.size() ; i++)
		{
			//cout << it->second[i] << " ";
			if (e == it->second[i])
				return;
		}
		
	}
	Map[f].push_back(e);
	exec("mkdir(others)");
	cout << endl;
}
// make the directory to store each file formate type in one file 
int main(int argc , char *argv[] )
{
	Map["Sandra"].push_back("asd");
	Map["Sandra"].push_back("asb");
	Map["Sandra"].push_back("aaa");
	Map["Sandy"].push_back("awa");
	Map["Sandy"].push_back("aaw");
	Map["MaRawan"].push_back("www");
	c("andrew","ll");
	map<string, vector<string> >::iterator it;
	for (it = Map.begin(); it != Map.end(); it++)
	{
		cout << it->first << " : ";
		for (int i = 0; i < it->second.size(); i++)
		{
			cout << it->second[i] << " ";
		}
		cout << endl;
		/*
		exec("");
		_FSTREAM_("extentionsfile");

		// Creating a directory
		/*if (mkdir("geeksforgeeks", 0777) == -1)
			cerr << "Error :  " << strerror(errno) << endl;

		else
			cout << "Directory created";
		exec(mkdir("NewFile"));
	}
	system("pause");
}
*/
	