/*
 * File: MyShellProcess.cpp
 *
 * The class for executing the commands
 *
 * Header:  MyShellProcess.h
 * Author: CSE410 Fall 2021
 *
 */

#include "MyShellProcess.h"
#include "MyShell.h"
#include "MyShellParser.h"
#include <string>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
using namespace std;

/*
 * Name :        MyShellProcess::MyShellProcess()
 * Description : Constructor that takes a pointer of MyShell
 */
MyShellProcess::MyShellProcess(MyShell *shell)
{
	mShell = shell;
}

/*
 * Name :        MyShellProcess::~MyShellProcess()
 * Description : Default destructor
 */
MyShellProcess::~MyShellProcess()
{
	// we do not need to delete mShell
	// since it is not created by "new"
	// in this class (it is passed in)
}

/*
 * Name :        MyShellProcess::ExecuteInternalCommand()
 * Description : This is where we really execute the internal command
 * Parameters :  a pointer to MyShellParser for retrieving the
 * 				 pre-processed commands
 * Returns:      void
 *
 */
void MyShellProcess::ExecuteInternalCommand(MyShellParser *parser)
{
	// get the arguments for executing
	char** args = parser->GetArguments(0);

	// store the first token of the command in cmd
	string cmd = args[0];

	//
	// check which internal command we are going to execute
	//

	// it's "hist"
	if(cmd == MyShellParser::Hist)
	{
		mShell->PrintHistory();
	}

	// it's "cd"
	if(cmd == MyShellParser::Cd)
	{
		// the usage of cd would be "cd Dir"
		// make sure that there are two arguments
		if(args[1] != NULL)
		{
			mShell->ChangeCurrentDirectory(args[1]);
		}
		else
		{
			cout<<"Internal Command Usage: cd <path>"<<endl;
		}
	}

	// it's "curr"
	if(cmd == MyShellParser::Curr)
	{
		cout<<mShell->GetCurrentDirectory()<<endl;
	}

	// it's "date"
	if(cmd == MyShellParser::Date)
	{
		cout<<mShell->GetTime();
	}

	// it's "quit"
	if(cmd == MyShellParser::Quit)
	{
		cout<<"exit!"<<endl;
		mShell->SetIsQuit(true);
	}
}

/*
 * Name :        MyShellProcess::ExecuteExternalCommand()
 * Description : This is where we really execute the external command
 * Parameters :  a pointer to MyShellParser for retrieving the
 * 				 pre-processed commands
 * Returns:      void
 */
void MyShellProcess::ExecuteExternalCommand(MyShellParser *parser)
{
	// we have to execute the external command in a new process
	// and if the commands end with "&", we have to execute it
	// in the background

	// if the external commands end with "&"
	if(parser->IsBackground())
	{
		ExecuteInBackground(parser);
	}
	// not end with "&", we simply execute
	// the command in a new process and make
	// the parent process wait for it.
	else
	{
		ExecuteInForeground(parser);
	}
}

/*
 * Name :        MyShellProcess::ExecuteInBackground()
 * Description : execute the external command in background
 * Parameters :  a pointer to MyShellParser for retrieving the
 * 				 pre-processed commands
 * Returns:      You have to implement this function
 */
void MyShellProcess::ExecuteInBackground(MyShellParser *parser)
{
	//
	// to do (replace the following line with your own code)
	//
	cout<<"Please implement MyShellProcess::ExecuteInBackground()"<<endl;
}

/*
 * Name :        MyShellProcess::ExecuteInForeground()
 * Description : execute the external command in foreground
 * Parameters :  a pointer to MyShellParser for retrieving the
 * 				 pre-processed commands
 * Returns:      void
 */
void MyShellProcess::ExecuteInForeground(MyShellParser *parser)
{
	// for storing the value returned from fork()
	pid_t pID;

	// now, we spawn a child process by invoking fork()
	// if pID < 0, there is something wrong when doing fork()
	if((pID=fork()) < 0)
	{
		cout<<"Fork() error"<<endl;
	}
	// in child process, we execute the command
	// by passing in a pointer of MyParser
	else if(pID == 0)
	{
		// the function for executing
		// commands stored in parser
		Execute(parser);

		// if we get to this line, there
		// is something wrong with Execute()
		exit(EXIT_FAILURE);
	}
	// in parent process
	else
	{
		// since we execute the command in foreground
		// we have to wait until the child process
		// terminates
		waitpid(pID, NULL, 0);
	}
}

/*
 * Name :        MyShellProcess::Execute()
 * Description : execute the external command in foreground
 * Parameters :  a pointer to MyShellParser for retrieving the
 * 				 pre-processed commands
 * Returns :     void
 * Note :        You have to implement this function
 */
void MyShellProcess::Execute(MyShellParser *parser)
{
	// given the parser containing all the
	// commands and delimiters you need for
	// doing all kinds of execution as follows
	// 	execute the command w/o IO redirection nor pipes
	// 	execute the command w/ pipe
	// 	execute the command w/ IO redirection

	//
	// to do (replace the following line with your own code)
	//
	cout<<"Please implement MyShellProcess::Execute()"<<endl;
}
