/*
 * File    : MyShellProcess.h
 *
 * Header for the MyShellProcess class for Project2.
 *
 * Class   : MyShellProcess.h
 * Impl    : MyShellProcess.cpp
 * Author: CSE410 Fall 2021
 *
 * Notes   : You have to implement some functions in this class
 */

#ifndef MYSHELLPROCESS_H_
#define MYSHELLPROCESS_H_

#include <string>

// forward reference
class MyShell;
class MyShellParser;

class MyShellProcess
{
public:
	MyShellProcess(MyShell *shell);
	virtual ~MyShellProcess();

	// functions for executing commands
	void ExecuteInternalCommand(MyShellParser* parser);
	void ExecuteExternalCommand(MyShellParser* parser);

private:
	// we will initialize a MyShellProcess by using default constructor
	// make it private
	MyShellProcess();

	// function required for executing
	void Execute(MyShellParser* parser);

	// execute the commands in background or foreground
	void ExecuteInBackground(MyShellParser* parser);
	void ExecuteInForeground(MyShellParser* parser);

	// execute pipelined commands, I/O commands, or combination commands
	void ExecutePipelinedCommands(MyShellParser* parser);
	void ExecuteIOCommands(MyShellParser* parser);
	void ExecuteCombinedCommands(MyShellParser* parser);

	std::string GetSourceFile(MyShellParser* parser);
	std::string GetDestinationFile(MyShellParser* parser);

	// for cooperating with MyShell object
	MyShell *mShell;
};

#endif /* MYSHELLPROCESS_H_ */
