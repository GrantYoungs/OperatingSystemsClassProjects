/*
 * File    : CShellParser.h
 *
 * Header for the CShellParser class for Project2.
 *
 * Class   : CShellParser.h
 * Impl    : CShellParser.cpp
 * Author: CSE410 Fall 2021
 *
 * Notes   : Do NOT modify this class
 */

#ifndef CSHELLPARSER_H_
#define CSHELLPARSER_H_
#include <vector>
#include <string>
#include "Parser.h"

class CShellParser : public Parser
{
public:
	CShellParser(const char** argv, const int argc);
	virtual ~CShellParser();

	// Getters
	char** GetArguments();
	int GetCommandCount(){return mCommands.size();}
	int GetArgCount(){return mCommands.size()+1;}

private:
	CShellParser();
	void Parse(const char** argv, const int argc);

	// store the tokenized commands
	std::vector<std::string>mCommands;
};

#endif /* CSHELLPARSER_H_ */
