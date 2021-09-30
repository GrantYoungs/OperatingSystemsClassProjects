/*
 * File    : RegularExpression.h
 *
 * Header for the RegularExpression class for Project2.
 *
 * Class   : RegularExpression.h
 * Impl    : RegularExpression.cpp
 * Created : Jan 10, 2012
 * Author  : CSE410 Spring 2012
 *
 * Notes   : Do NOT modify this class
 */

#ifndef REGULAREXPRESSION_H_
#define REGULAREXPRESSION_H_
#include <regex.h>
#include <string>

class RegularExpression
{
public:
	RegularExpression(const std::string & exp);
	virtual ~RegularExpression();
	int GetCompileValue() const {return mCompileValue;}
	bool Match(const std::string &s);
private:
	void Compile();
	RegularExpression();

	std::string mSearchExpression;
	regex_t mRegexValue;
	int mCompileValue;
};

#endif /* REGULAREXPRESSION_H_ */
