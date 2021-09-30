/*
 * File: RegularExpression.cpp
 *
 * A class wraps up the functionality of checking the input pattern.
 * We passed a expression to the constructor when creating the object.
 * To check whether the input string matches our configured pattern,
 * we can use Match() function, which will return true if the target
 * string complies to our desired pattern.
 *
 * Header:  MyShellProcess.h
 * Author: CSE410 Spring 2012
 *
 */

#include "RegularExpression.h"
using namespace std;

/*
 * Name :        RegularExpression::RegularExpression
 * Description : Constructor that takes a string representing
 * 				 our desired pattern
 */
RegularExpression::RegularExpression(const std::string & exp)
{
	mSearchExpression = exp;
	Compile();
}

/*
 * Name :        RegularExpression::~RegularExpression()
 * Description : Default destructor
 */
RegularExpression::~RegularExpression()
{
}

/*
 * Name :        RegularExpression::Match()
 * Description : To check whether a string matches our configured pattern,
 * 				 we can use this function which will
 * Parameters :  a string we are going to check with our desired mattern
 * Returns:      true if the target string (s) complies to our desired pattern.
 *
 */
bool RegularExpression::Match(const std::string & s)
{
	bool flag = true;
	if(GetCompileValue() != 0)
	{
		flag = false;
	}
	else
	{
		regmatch_t match;
		regexec(&mRegexValue, s.c_str(), 1, &match, 0);

		// check length
		if((match.rm_so != 0) || (match.rm_eo != int(s.size())))
		{
			flag = false;
		}
	}
	return flag;
}

/*
 * Name :        RegularExpression::Compile()
 * Description : invoke regcomp()
 * Parameters :  void
 * Returns:      void
 *
 */
void RegularExpression::Compile()
{
	mCompileValue = regcomp(&mRegexValue, mSearchExpression.c_str(), REG_EXTENDED);
}



