/*
 * File    : MyShellParser.h
 *
 * Header for the MyShellParser class for Project2.
 *
 * Class   : MyShellParser.h
 * Impl    : MyShellParser.cpp
 * Created : Jan 10, 2012
 * Author  : CSE410 Spring 2012
 *
 * Notes   : Do NOT modify this class
 */

#ifndef MYSHELLPARSER_H_
#define MYSHELLPARSER_H_
#include "RegularExpression.h"
#include "Parser.h"
#include <vector>


class MyShellParser : public Parser
{
public:
	// internal commands
    static const std::string Hist; 			// "hist"
    static const std::string Cd; 			// "cd"
    static const std::string Curr; 			// "curr"
    static const std::string Date; 			// "date"
    static const std::string Quit; 			// "quit"

    // delimiters
    static const std::string Amp; 			// '&', ampersand
    static const std::string Pipe; 			// '|'
    static const std::string RedirStdout; 	// '>'
    static const std::string RedirStdin; 	// '<'

    // max length of each command line input
    static const int MaxCommandLength;		// 128

    static const std::string WhiteSpaces;
    static const std::string CommandDelimiters;

	MyShellParser(const std::string &cmd);
	virtual ~MyShellParser();

	bool ValidateCommand();
	bool Parse();
	void CheckBackgroundCMD();
	void Trim(std::string &s);

	// Getters
	char** GetArguments(const int index);
	std::string GetDelimiter(const int index);
	std::string GetCommand(const int index);
	int GetCommandCount() const {return mCommands.size();}
	int GetDelimiterCount() const {return mDelimiters.size();}
	std::string GetFirstCommand();
	bool IsInternalCommand();
	bool IsBackground() const {return mIsBackground;}

private:
	// store the initial user input from command line
	std::string mUserInput;

	// store the tokenized commands
	std::vector<std::string> mCommands;

	// store delimiters between commands
	std::vector<std::string> mDelimiters;

	// should we run the command in the background
	bool mIsBackground;

	// for checking whether the user input is valid
	// by invoking the Match() through RegularExpression objects
	RegularExpression *mCommandRegularExpression;
	RegularExpression *mBackgroundCMDRegularExpression;
};

#endif /* MYSHELLPARSER_H_ */
