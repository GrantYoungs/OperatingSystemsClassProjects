/*
 * File: MyShellParser.cpp
 *
 * The class for parsing command line input of MyShell
 *
 * Header:  MyShellParser.h
 * Author: CSE410 Fall 2021
 *
 */

#include "MyShellParser.h"
#include <string.h>
#include <iostream>
#include <stdio.h>
using namespace std;


// internal commands
const string MyShellParser::Hist = "hist";
const string MyShellParser::Cd = "cd";
const string MyShellParser::Curr = "curr";
const string MyShellParser::Date = "date";
const string MyShellParser::Quit = "quit";

// delimiters
const string MyShellParser::Amp = "&";
const string MyShellParser::Pipe = "|";
const string MyShellParser::RedirStdout = ">";
const string MyShellParser::RedirStdin = "<";

// max command line input length
const int MyShellParser::MaxCommandLength = 128;

// command line delimiters
const string MyShellParser::CommandDelimiters = MyShellParser::Amp +
		MyShellParser::Pipe + MyShellParser::RedirStdout +
		MyShellParser::RedirStdin;

const string MyShellParser::WhiteSpaces = " \t\n\v\f\r";

/*
 * Regular validCommandExpression to match valid command strings.  A valid command
 * consists of the following pattern, with any amount of spaces interspersed:
 *
 * 		(word)+ (ioRedirection word+)+
 *
 * That is to say, that there must be at least one 'word', and it can
 * stand alone, OR it can be followed by an IO redirection symbol, which
 * MUST be followed by another word.
 *
 * A 'word' is defined as any non-whitespace character that is also not
 * in the set of delimiters "><|&".
 *
 * IO redirection symbols are defined as a subset of the delimiters, "><|".
 *
 * Finally, a command can be optionally followed by an ampersand (see above).
 *
 * Alternately, it is valid to have a command that consists entirely of
 * whitespace.
 *
 * for more info about Regular Expression, please visit
 * the following site for more details
 * http://www.tropicsoft.com/Components/RegularExpression/
 * http://www.zytrax.com/tech/web/regex.htm
 */
const string RE_Word = "[^[:space:]"+ MyShellParser::CommandDelimiters + "]";
const string RE_Space = "[[:space:]]*";
const string RE_Token = RE_Space + RE_Word + RE_Space;
const string RE_AmpEnd = "([" + MyShellParser::Amp +"]" + RE_Space+")?";
const string RE_IORedir = "["+ MyShellParser::RedirStdin + MyShellParser::RedirStdout +
							MyShellParser::Pipe +"]";

const string RE_ValidCommand = "^((" + RE_Token + ")+(" + RE_IORedir + "(" + RE_Token +
								")+)*" + RE_AmpEnd + "$)|(^" + RE_Space + "$)";

const string RE_BackgroundCommand = "^[^&]*&[[:space:]]*$";

/*
 * Name :        MyShellParser::MyShellParser()
 * Description : Constructor that takes a string (command line input) as input
 */
MyShellParser::MyShellParser(const std::string &cmd)
{
	mUserInput = cmd;
	mIsBackground = false;
	mCommandRegularExpression = new RegularExpression(RE_ValidCommand);
	mBackgroundCMDRegularExpression = new RegularExpression(RE_BackgroundCommand);
}

/*
 * Name :        MyShellParser::~MyShellParser()
 * Description : Default destructor
 */
MyShellParser::~MyShellParser()
{
	// clean after ourselves
	delete mCommandRegularExpression;
	delete mBackgroundCMDRegularExpression;
}

/*
 * Name :        MyShellParser::ValidateCommand()
 * Description : check the user input is valid
 * Parameters :  void
 * Returns:      void
 *
 */
bool MyShellParser::ValidateCommand()
{
	bool flag = true;

	// make sure that user did type something
	// but no more than the MaxCommandLength
	const int cmdSize = mUserInput.size();
	if( cmdSize == 0 || cmdSize>MaxCommandLength)
	{
		flag = false;
	}
	else // check if the command is valid (matches our desired pattern)
	{
		flag = mCommandRegularExpression->Match(mUserInput);
	}
	return flag;
}

/*
 * Name :        MyShellParser::Parse()
 * Description : tokenize the user input and store the results
 *               in mCommands and mDelimiters separately
 * Parameters :  void
 * Returns:      void
 *
 */
bool MyShellParser::Parse()
{
	// the offsets of the whole command line input
	size_t pos = 0;
	size_t lastPos = 0;

	bool flag = true;
	bool isDone = false;

	while(!isDone)
	{
		string token;
		string delimiter;

		// Search for the next delimiter
		pos = mUserInput.find_first_of(CommandDelimiters, pos);

        // If no delimiter was found, just grab the text.
        if (pos == string::npos)
        {
            // Grab the command
            token = mUserInput.substr(lastPos);

            // we are done
            isDone = true;
        }
        // If a delimiter was found, grab the delimiter *and* the text.
        else
        {
            // Grab the delimiter character, and the text
            delimiter = mUserInput.substr(pos, 1);
            token = mUserInput.substr(lastPos, pos - lastPos);

            // add the delimiter but NOT "&"
            if(delimiter != MyShellParser::Amp) mDelimiters.push_back(delimiter);
        }
        // add the command token
        // before adding the command token we remove all the spaces
        // before and after the command
        Trim(token);

        // we add the command only if the size of
        // the trimmed command > 0
        if(token.size() > 0)
        {
        	mCommands.push_back(token);
        }

        // Increment the position values
        pos++;
        lastPos = pos;
	}

	if(flag)
	{
		// Check to make sure the numbers all match up.
		// There should be the same, or one more, command
	    // than there are delimiters:
	    // command DELIM command DELIM command [optional & delim]
		const int tokenNumber = mCommands.size();
		const int deliNumber = mDelimiters.size();
	    if(tokenNumber != deliNumber && tokenNumber != (deliNumber+1))
	    {
	        flag = false;
	    }

		// Background command?
	    CheckBackgroundCMD();
	}
	return flag;
}

/*
 * Name :        MyShellParser::CheckBackgroundCMD()
 * Description : check if the command is for running in background
 *               (ends with "&" )
 * Parameters :  void
 * Returns:      void
 *
 */
void MyShellParser::CheckBackgroundCMD()
{
	mIsBackground = mBackgroundCMDRegularExpression->Match(mUserInput);
}

/*
 * Name :        MyShellParser::Trim()
 * Description : remove spaces before and after the given string
 * Parameters :  a reference to the string we are going to remove
 * 				 spaces for
 * Returns:      void
 *
 */
void MyShellParser::Trim(std::string &s)
{
    // Trim Both leading and trailing spaces
	size_t startPos = s.find_first_not_of(MyShellParser::WhiteSpaces); // Find the first character position after excluding leading blank spaces
	size_t endPos = s.find_last_not_of(MyShellParser::WhiteSpaces); // Find the first character position from reverse af

    // if all spaces or empty return an empty string
    if ((string::npos == startPos) || (string::npos == endPos))
    {
        s = "";
    }
    else
    {
        s = s.substr(startPos, endPos - startPos + 1);
    }
}

/*
 * Name :        MyShellParser::GetFirstCommand()
 * Description : get the first command from mCommands
 * Parameters :  void
 * Returns:      void
 *
 */
std::string MyShellParser::GetFirstCommand()
{
	// make sure that there is something in our mCommands
	if(GetCommandCount() > 0) return mCommands[0];
	else
	{
		cout<<"Empty mTokens in MyShellParser!!"<<endl;
		return "";
	}
}

/*
 * Name :        MyShellParser::IsInternalCommand()
 * Description : check if the command from the user is of
 * 				 type internal command
 * Parameters :  void
 * Returns:      true if it is an internal command
 *
 */
bool MyShellParser::IsInternalCommand()
{
	bool flag = false;
	if(GetCommandCount() > 0)
	{
		string firstCMD = GetFirstCommand();
		size_t pos = firstCMD.find_first_of(MyShellParser::WhiteSpaces, 0);
		if(pos != string::npos)
		{
			firstCMD = firstCMD.substr(0, (pos - 0));
		}

		// check if the command matches any of the internal commands
		if(firstCMD == MyShellParser::Hist){flag = true;}
		if(firstCMD == MyShellParser::Cd){flag = true;}
		if(firstCMD == MyShellParser::Curr){flag = true;}
		if(firstCMD == MyShellParser::Date){flag = true;}
		if(firstCMD == MyShellParser::Quit){flag = true;}
	}
	return flag;
}

/*
 * Name :        MyShellParser::GetArguments()
 * Description : convert the command stored in mCommands
 *               into char** for executing
 * Parameters :  the index of the command stored in mCommands
 * 				 that you want to convert into char**
 * Returns:      char**
 *
 */
char **MyShellParser::GetArguments(const int index)
{
	if(index >= GetCommandCount())
	{
		cout<<"Cannot GetArguments in MyShellParser::GetArguments()"<<endl;
		return NULL;
	}

	vector<string>CMDTokens;
	const string currentCMD = mCommands[index];
	size_t pos = 0;
	size_t lastPos = 0;

	// tokenize the current command
    while (pos != string::npos)
    {
        pos = currentCMD.find_first_of(MyShellParser::WhiteSpaces, lastPos);

        string token;

        if(pos != string::npos)
        {
        	token = currentCMD.substr(lastPos, (pos - lastPos));
        }
        else
        {
        	token = currentCMD.substr(lastPos);
        }
        // remove spaces
        Trim(token);

        if (token.size() > 0)
        {
        	CMDTokens.push_back(token);
        }

        lastPos = pos + 1;
    }

	// theArgs is a pointer to an array of pointers to
	// null-terminated character strings.
    const int tokenNumber = CMDTokens.size();
	char** theArgs = new char*[tokenNumber+1];

	for(int i = 0; i<tokenNumber; i++)
	{
        string t = CMDTokens[i];

        if(!IsInternalCommand() && i == 0)
        {
        	string tempPath = FindPath(t, BinPath);
        	if(tempPath == "") tempPath = FindPath(t, UsrBinPath);
    		// concatenate the bin path with the command
    		t = tempPath + t;
        }


        theArgs[i] = new char[t.size() + 1];
        strncpy(theArgs[i], t.c_str(), t.size() + 1);
	}

	theArgs[tokenNumber] = NULL;

	return theArgs;
}

/*
 * Name :        MyShellParser::GetDelimiter()
 * Description : get the delimiter stored in mDelimiters
 * Parameters :  the index of the delimiter stored in mDelimiters
 * 				 that you want to get
 * Returns:      string
 *
 */
std::string MyShellParser::GetDelimiter(const int index)
{
	// make sure that we did not go out of the bound
	if(index >= GetDelimiterCount())
	{
		cout<<"Cannot GetDelimiter in MyShellParser::GetDelimiter()"<<endl;
		return "";
	}
	return mDelimiters[index];
}

/*
 * Name :        MyShellParser::GetCommand()
 * Description : get the command stored in mCommands
 * Parameters :  the index of the command stored in mCommands
 * 				 that you want to get
 * Returns:      string
 *
 */
std::string MyShellParser::GetCommand(const int index)
{
	if(index >= GetCommandCount())
	{
		cout<<"Cannot GetCommand in MyShellParser::GetCommand()"<<endl;
		return "";
	}
	return mCommands[index];
}
