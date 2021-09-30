/*
 * File: CShellParser.cpp
 *
 * The class for parsing command line input when user invoking myShell
 * in the following way:
 *
 * 		myShell -c <commands>
 *
 * Header:  MyShellParser.h
 * Author: CSE410 Fall 2021
 *
 */

#include "CShellParser.h"
#include <iostream>
#include <string.h>
using namespace std;

/*
 * Name :        CShellParser::CShellParser
 * Description : Constructor that takes a string (command line input) and
 *  			 an int for number of arguments as input
 */
CShellParser::CShellParser(const char** argv, const int argc)
{
	Parse(argv, argc);
}

/*
 * Name :        CShellParser::~CShellParser()
 * Description : Default destructor
 */
CShellParser::~CShellParser()
{
}

/*
 * Name :        CShellParser::Parse()
 * Description : tokenize the user input and store the results
 *               in mCommands
 * Parameters :  char** user command line input
 * 				 int argc the number of arguments
 * Returns:      void
 *
 */
void CShellParser::Parse(const char **argv, const int argc)
{
	for(int i = 0; i<argc; i++)
	{
		string cmd = argv[i];
		mCommands.push_back(cmd);
	}
}

/*
 * Name :        CShellParser::GetArguments()
 * Description : convert the command stored in mCommands
 *               into char** for executing
 * Parameters :  the index of the command stored in mCommands
 * 				 that you want to convert into char**
 * Returns:      char**
 *
 */
char **CShellParser::GetArguments()
{
	const int commandNumber = mCommands.size();

	// theArgs is a pointer to an array of pointers to
	// null-terminated character strings.
	char** theArgs = new char*[commandNumber+1];

	for(int i = 0; i< commandNumber; i++)
	{
		string currentCMD = mCommands[i];

		// concatenate the bin path with the command
		if(i == 0)
		{
        	string tempPath = FindPath(currentCMD, BinPath);
        	if(tempPath == "") tempPath = FindPath(currentCMD, UsrBinPath);
			currentCMD = tempPath + currentCMD;
		}
		//convert the string into null terminated C string
		const int cmdLength = currentCMD.size();
		theArgs[i] = new char[cmdLength+1];
		strncpy(theArgs[i], currentCMD.c_str(), cmdLength+1);
	}

	//A NULL pointer is used to mark the end of the array.
	theArgs[commandNumber] = NULL;

	return theArgs;
}
