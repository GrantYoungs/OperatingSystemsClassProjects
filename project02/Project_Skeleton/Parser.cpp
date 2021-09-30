/*
 * File: Parser.cpp
 *
 * The base class for all other parsers
 *
 * Header:  Parser.h
 * Author: CSE410 Fall 2021
 *
 */

#include "Parser.h"
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
using namespace std;

// search paths for external commands
const string Parser::BinPath = "/bin/";
const string Parser::UsrBinPath = "/usr/bin/";

/*
 * Name :        Parser::Parser()
 * Description : Default constructor
 */
Parser::Parser()
{

}

/*
 * Name :        MyShellParser::~MyShellParser()
 * Description : Default destructor
 */
Parser::~Parser()
{

}

/*
 * Name :        Parser::FindPath()
 * Description : get the correct path for the command
 * Parameters :  cmd - the command we are looking for
 * 			     searchPath - the path where we can find the command
 * Returns:      string
 *
 */
std::string Parser::FindPath(const std::string cmd, const std::string searchPath)
{
	int pathSize = searchPath.size();
	string pathString = searchPath;
	if(searchPath[pathSize-1] != '/') pathString+='/';

	DIR *dir = NULL;
	dir = opendir(searchPath.c_str());
	if(dir == NULL)
	{
		cerr<<"Open search path Failed!"<<endl;
	}
	else
	{
		struct dirent* dirEntry = NULL;

		while((dirEntry = readdir(dir)))
		{
			string currentFileName = dirEntry->d_name;
			string currentFullFileName = pathString+currentFileName;
			const char* statFullFileName = currentFullFileName.c_str();

			struct stat currentFileStat;

			if(stat(statFullFileName, &currentFileStat) == -1)
			{
				continue;
			}
			else
			{
				bool isFound = false;
				if(currentFileName == cmd)
				{
					isFound = true;
					return pathString;
					break;
				}
			}
		}
	}
	return "";
}
