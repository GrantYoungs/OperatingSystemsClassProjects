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
#include <fcntl.h>
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
	pid_t pID;

	if ((pID = fork()) < 0)
	{
		cout << "fork() error" << endl;
	}
	else if (pID == 0)
	{
		// Inside the child process
		Execute(parser);
		exit(EXIT_FAILURE);
	}

	// Because this command is meant to be ran in the background,
	// the parent process does not have to wait for the child process to finish.
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
	// 	execute the command without IO redirection nor pipes
	// 	execute the command w/ pipe
	// 	execute the command w/ IO redirection

	int delimiterCount = parser->GetDelimiterCount();
	
	// If there are no delimiters ( |, <, > ), then simply execute the command
	if (delimiterCount == 0)
	{
		// If there are no delimiters, then there will only be one set of arguments,
		// retrievable at the index 0
		char** argv = parser->GetArguments(0);
		execv(argv[0], argv);
	}

	// Otherwise, we're going to have to work with some delimiters.
	else
	{

		// First thing we need to do is determine if the command list is only pipelined,
		// only I/O, or a combination of both.
		bool pipeline = false;
		bool io = false;

		for (int i = 0; i < delimiterCount; i++)
		{
			// cout << "Delimiter index " << i << " " << parser->GetDelimiter(i) << endl;
			
			if (parser->GetDelimiter(i) == MyShellParser::Pipe)
			{
				pipeline = true;
			}

			if (parser->GetDelimiter(i) == MyShellParser::RedirStdin
				|| parser->GetDelimiter(i) == MyShellParser::RedirStdout)
			{
				io = true;
			}
		}

		if (pipeline && io)
		{
			ExecuteCombinedCommands(parser);
		}
		else if (pipeline)
		{
			ExecutePipelinedCommands(parser);
		}
		else if (io)
		{
			ExecuteIOCommands(parser);
		}
	}
}

/*
 * Name :        MyShellProcess::ExecutePipelinedCommands()
 * Description : execute a command that only has pipelines
 * Parameters :  a pointer to MyShellParser for retrieving the
 * 				 pre-processed commands
 * Returns :     void
 */
void MyShellProcess::ExecutePipelinedCommands(MyShellParser* parser)
{
	// Save in / out
	int default_in = dup(STDIN_FILENO);
	int default_out = dup(STDOUT_FILENO);

	int fdin = dup(default_in);

	pid_t pID;
	int fdout;

	for (int i = 0; i < parser->GetCommandCount(); i++)
	{
		// Redirect input
		dup2(fdin, 0);
		close(fdin);

		char** argv = parser->GetArguments(i);

		// If we're on the last command
		if (i == parser->GetCommandCount() - 1)
		{
			// Use the default output
			fdout = dup(default_out);
		}
		else
		{
			// Not the last command, keep piping
			int fdpipe[2];
			pipe(fdpipe);
			fdout = fdpipe[STDOUT_FILENO];
			fdin = fdpipe[STDIN_FILENO];
		}

		// Redirect output
		dup2(fdout, STDOUT_FILENO);
		close(fdout);

		// Create the child process
		if ((pID = fork()) < 0)
		{
			cout << "fork() error" << endl;
		}
		else if (pID == 0)
		{
			// In the child process
			execv(argv[0], argv);

			// If we get to this point, something went wrong
			exit(EXIT_FAILURE);
		}
	} // end for

	// Restore in / out defaults
	dup2(default_in, STDIN_FILENO);
	dup2(default_out, STDOUT_FILENO);
	close(default_in);
	close(default_out);

	// Wait for all the child processes created for the separate
	// pipelined commands to finish
	waitpid(pID, NULL, 0);
}

/*
 * Name :        MyShellProcess::ExecuteIOCommands()
 * Description : execute a command that only has I/O redirection
 * Parameters :  a pointer to MyShellParser for retrieving the
 * 				 pre-processed commands
 * Returns :     void
 */
void MyShellProcess::ExecuteIOCommands(MyShellParser* parser)
{
	// For this, we need to determine the input source and the output destination.
	// The input source will be provided by the command set immediately following a <.
	// The output destination will be provided by the command immediately following a >.

	string source = GetSourceFile(parser);
	string destination = GetDestinationFile(parser);

	int inputfd;
	if (source != "")
	{
		// If it fails to open the input file
		if ((inputfd = open(source.c_str(), O_RDONLY | O_CREAT)) < 0)
		{
			cout << "Error: " << errno << endl;
		}
		else
		{
			// Otherwise, copy the inputfd over to STDIN,
			// then close inputfd to free space.
			dup2(inputfd, STDIN_FILENO);
			close(inputfd);
		}
	}

	int outputfd;
	if (destination != "")
	{
		if ((outputfd = open(destination.c_str(), O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO)) < 0)
		{
			cout << "Error: " << errno << endl;
		}
		else
		{
			dup2(outputfd, STDOUT_FILENO);
			close(outputfd);
		}
	}

	char ** argv = parser->GetArguments(0);
	execv(argv[0], argv);
	exit(EXIT_FAILURE);
}

/*
 * Name :        MyShellProcess::ExecuteCombinedCommands()
 * Description : execute a command that has both pipes and I/O redirection
 * Parameters :  a pointer to MyShellParser for retrieving the
 * 				 pre-processed commands
 * Returns :     void
 */
void MyShellProcess::ExecuteCombinedCommands(MyShellParser* parser)
{
	cout << "Combined I/O and Piped commands for extra credit have not yet been implemented." << endl;
}

/*
 * Name:			MyShellProcess::GetSourceFile()
 * Description:		Returns the name of a source file input provided in a
 * 					MyShellParser object arguments list.
 * Parameters:		A pointer to MyShellParser for retrieving the
 * 					source file input.
 * Returns:			The source file input, or an empty string.
 */
std::string MyShellProcess::GetSourceFile(MyShellParser* parser)
{
	string source = "";
	for (int i = 0; i < parser->GetDelimiterCount(); i++)
	{
		string delim = parser->GetDelimiter(i);
		if (delim == MyShellParser::RedirStdin)
		{
			// < is input
			source = parser->GetCommand(i + 1);
			break;
		}
	}
	return source;
}

/*
 * Name:			MyShellProcess::GetDestinationFile()
 * Description:		Returns the name of a destination file output provided in a
 * 					MyShellParser object arguments list.
 * Parameters:		A pointer to MyShellParser for retrieving the
 * 					destination file output.
 * Returns:			The destination file output, or an empty string.
 */
std::string MyShellProcess::GetDestinationFile(MyShellParser* parser)
{
	string destination = "";
	for (int i = 0; i < parser->GetDelimiterCount(); i++)
	{
		string delim = parser->GetDelimiter(i);
		if (delim == MyShellParser::RedirStdout)
		{
			// > is output
			destination = parser->GetCommand(i + 1);
			break;
		}
	}
	return destination;
}
