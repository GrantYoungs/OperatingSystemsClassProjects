/*
 * File: MyShell.cpp
 *
 * The class for prompting and handling user input
 *
 * Header:  MyShell.h
 * Author: CSE410 Fall 2021
 *
 */

#include "MyShell.h"
#include "MyShellParser.h"
#include "MyShellProcess.h"
#include "CShellParser.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <iostream>
#include <errno.h>
#include <unistd.h>

using namespace std;

// keep 10 most recent input lines in the history vector
const int HistorySize = 10;

/*
 * Name :        MyShell::MyShell()
 * Description : Default constructor
 */
MyShell::MyShell()
{
	//initialize all the memeber variables
	mHostName = GetHostName();
	mUserName = GetUserName();
	mSequenceNumber = 1;
	mIsQuit = false;

	// set history counter to 1 (since the first command line stored
	// in the history vector will start with the sequence number equal to 1
	mHistoryCounter = 1;
}

/*
 * Name :        MyShell::~MyShell()
 * Description : Default destructor
 */
MyShell::~MyShell()
{

}


/*
 * Name :        MyShell::ExecuteCShellCommand()
 * Description : handle the command from CShell (i.e. myShell -c <command>)
 * Parameters :  a pointer to CShellParser
 *
 * Note	:    You have to implement this function
 */
void MyShell::ExecuteCShellCommand(CShellParser *u)
{
	// retrieve the args for executing
	char** myArgs = u->GetArguments();

	//
	// to do (replace the following line with your own code)
	//
	cout<<"Please implement  MyShell::ExecuteCShellCommand()"<<endl;
}

/*
 * Name :        MyShell::Prompt()
 * Description : keep displaying a prompt and accepting user input
 * Parameters :
 *
 * Note	:    You have to implement this function
 */
void MyShell::Prompt()
{
	// keep prompting if IsQuit() is false
	while(!IsQuit())
	{
		// print <1 arctic:userName>
		PrintPromptInfo();

		string currentCMD;
		// read in user input
		getline(cin, currentCMD);

		//====================================================================
		// to do (replace the following line with your own code)
		//
		cout<<"Update the history of command line input"<<endl;



		// end of to do
		//====================================================================

		// use the parser for analyzing
		// the user input from the command line
		MyShellParser myParser(currentCMD);

		// something wrong with the command
		// we print the error message
		if(!myParser.ValidateCommand() || !myParser.Parse())
		{
			cout<<"Syntax Error: Invalid Commands"<<endl;
		}
		// command is valid, we can execute it
		else
		{
			ExecuteMyShellCommand(&myParser);
		}
	}

	// before leaving our shell, we wait
	// for all the running child processes until
	// they terminate
	WaitForChildProcess();

}

/*
 * Name :        MyShell::PrintPromptInfo()
 * Description : printing the prompt info <1 arctic:userName>
 * Parameters :  No
 *
 * Note	:    You have to implement this function
 */
void MyShell::PrintPromptInfo()
{
	//
	// to do (replace the following line with your own code)
	//
	cout<<"<PrintPromptInfo()>";
}

/*
 * Name :        PrintHistory()
 * Description : displaying the history
 * Parameters :  No
 *
 * Note	:    You have to implement this function
 */
void MyShell::PrintHistory()
{
	//
	// to do (replace the following line with your own code)
	//
	cout<<"Please implement MyShell::PrintHistory()"<<endl;
}

/*
 * Name :        MyShell::UpdateHistory()
 * Description : Update the history of command line inputs
 * Parameters :  string of the current command line input
 *
 * Note	:    You have to implement this function
 */
void MyShell::UpdateHistory(const std::string & h)
{
	//
	// to do (replace the following line with your own code)
	//
	cout<<"You have to implement MyShell::UpdateHistory()"<<endl;
}

/*
 * Name :        MyShell::ExecuteMyShellCommand()
 * Description : execute the command
 * Parameters :  parser - a pointer pointing to MyShellParser object
 */
void MyShell::ExecuteMyShellCommand(MyShellParser *parser)
{
	// the number of commands for executing
	int tokenCount = parser->GetCommandCount();

	// no commands, we are done
	if( tokenCount == 0) return;

	// see if internal commands
	if(parser->IsInternalCommand())
	{
		ExecuteInternalCommand(parser);
	}
	// external commands
	else
	{
		ExecuteExternalCommand(parser);
	}
}

/*
 * Name :        MyShell::ExecuteInternalCommand()
 * Description : This is where MyShell invokes MyShellProcess to execute
 * 				 the internal command (i.e. hist, cd Dir, curr, date, quit)
 * Parameters :  a pointer to MyShellParser, used for retrieving the
 * 				 pre-processed commands
 *
 */
void MyShell::ExecuteInternalCommand(MyShellParser *parser)
{
	// create a MyShellProcess project for executing the command
	// by passing a pointer of MyShellParser to the function invoked
	// as follows
	MyShellProcess myProcess(this);
	myProcess.ExecuteInternalCommand(parser);
}

/*
 * Name :        MyShell::ExecuteExternalCommand()
 * Description : This is where MyShell invokes MyShellProcess to execute
 * 				 the external command (anything that is not part of
 * 				 hist, cd Dir, curr, date, quit)
 * Parameters :  a pointer to MyShellParser, used for retrieving the
 * 				 pre-processed commands
 *
 */
void MyShell::ExecuteExternalCommand(MyShellParser *parser)
{
	// create a MyShellProcess project for executing the command
	// by passing a pointer of MyShellParser to the function invoked
	// as follows
	MyShellProcess myProcess(this);
	myProcess.ExecuteExternalCommand(parser);
}

/*
 * Name :        MyShell::GetUserName()
 * Description : get the current user name, which will be printed out in
 * 				 prompt (e.g. <1 arctic:userName> )
 * Returns :     string of the username (e.g. userName)
 * Note	:    You have to implement this function
 */
std::string MyShell::GetUserName()
{
	//
	// to do (replace the following line with your own code)
	//
	return "MyShell::GetUserName()";
}

/*
 * Name :        MyShell::GetHostName()
 * Description : get the current host name, which will be printed out in
 * 				 prompt (e.g. <1 arctic:userName> )
 * Returns :     string of the host name (e.g. arctic)
 * Note	:    You have to implement this function
 */
std::string MyShell::GetHostName()
{
	//
	// to do (replace the following line with your own code)
	//
	return "MyShell::GetHostName()";
}

/*
 * Name :        MyShell::GetTime()
 * Description : get the current local time for the internal command "date"
 * Parameters :
 * Returns :     string of the time (e.g. Wed Jan 11 10:13:38 2012)
 * Note	:    You have to implement this function
 */
std::string MyShell::GetTime()
{
	//
	// to do (replace the following line with your own code)
	//
	return "MyShell::GetTime()";
}

/*
 * Name :        MyShell::GetCurrentDirectory()
 * Description : get the current local time for the internal command "date"
 * Returns :     string of the path of current directory
 * 				(e.g. /home/student/Desktop/CSE410_Spring2012/Project2)
 * Note	:    You have to implement this function
 */
std::string MyShell::GetCurrentDirectory()
{
	//
	// to do (replace the following line with your own code)
	//
	return "MyShell::GetCurrentDirectory()";
}

/*
 * Name :        MyShell::ChangeCurrentDirectory()
 * Description : move from the current directory to the new directory
 * 				 specified in "dir"
 * Parameters :  char* dir, the new directory we want to change to
 * Returns :     void
 * Note	:    You have to implement this function
 */
void MyShell::ChangeCurrentDirectory(const char* dir)
{
	//
	// to do (replace the following line with your own code)
	//
	cout<<"You have to implement MyShell::ChangeCurrentDirectory()"<<endl;
}

/*
 * Name :        MyShell::WaitForChildProcess()
 * Description : invoked before exiting our shell to wait for still running
 * 				 child processes
 * Parameters :  void
 * Returns :     void
 */
void MyShell::WaitForChildProcess()
{
	// keep waiting untill all child processes terminate
	pid_t cPID;
	while ((cPID = waitpid(-1, NULL, 0)) > 0)
	{
	   if (errno == ECHILD)
	   {
	      break;
	   }
	}
}
