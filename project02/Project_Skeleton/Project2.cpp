//============================================================================
// Name        : Project2.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : CSE410 Project2 Template
//============================================================================

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CShellParser.h"
#include "MyShell.h"
using namespace std;

// cshell command option
const string OptionC = "-c";

/*
 * Name :        main()
 * Description : the main function of the program.
 * Parameters :  command line input for search path, option and target
 * Returns :     0 if the program runs successfully.
 *               1 if error occurs.
 */
int main(int argc, const char* argv[])
{
	// check if the user invoke the program
	// together with command line arguments
	// e.g. "./myShell -c <command>"
	if(argc >= 2)
	{
		string option = argv[1];
		if(option.compare(OptionC) == 0 && argc > 2)
		{
			// ignore the first two inputs from the
			// command line (myShell and -c)
			CShellParser parser(&argv[2], argc-2);

			// Myshell object
			MyShell shell;

			// make the shell execute the cshell command
			// you have to implement this function in MyShell
			shell.ExecuteCShellCommand(&parser);
		}
		else
		{
			cerr<<"Usage: myShell -c <command> or myShell (only)"<<endl;
			return 1;
		}
	}
	// No command line input
	// the user invoke myShell directly
	else
	{
		// Myshell object
		MyShell shell;

		// start prompting
		shell.Prompt();
	}
	return 0;
}


