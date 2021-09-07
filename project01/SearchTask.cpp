/*
 * File: SearchTasl.cpp
 *
 * The class for handling the command line inputs
 *
 * Header: SearchTask.h
 * Author: CSE410 Spring 2012
 *
 * Notes: Do not modify this class
 */

#include "SearchTask.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

// number of characters of different formats
static const unsigned int DateCharNumber = 17;
static const unsigned int PermissionCharNumber = 3;

/*
 * Name :        SearchTask::SearchTask()
 * Description : Default constructor
 */
SearchTask::SearchTask()
{

}

/*
 * Name :        SearchTask::~SearchTask()
 * Description : Default destructor
 */
SearchTask::~SearchTask()
{

}

/*
 * Name :        SearchTask::ProcessOptionTarget()
 * Description : Initialize our SearchTask object
 * Parameters :  string - option (-name, -size, etc)
 * 				 string - target (myFile, 755, 12:22:35-01/02/12, etc)
 * Returns :     true if no errors
 */
bool SearchTask::ProcessOptionTarget(const std::string option, const std::string target)
{
	// set up the type of our searchtask
	bool flag = SetType(option);
	if(!flag)
	{
		return flag;
	}
	else
	{
		// after type is set, we set the target
		SetStringTarget(target);
		flag = CheckValidity();
		if(!flag) return flag;
	}

	return flag;
}

/*
 * Name :        SearchTask::SetType()
 * Description : set the type of the current object
 * Parameters :  the user command line input string
 * Returns :     true if the type matches our defined types
 */
bool SearchTask::SetType(const string s)
{
	bool flag = true;

	// set the type according to the user input
	if(s.compare("-name") == 0)
	{
		mType = Name;
	}
	else if(s.compare("-size") == 0)
	{
		mType = Size;
	}
	else if(s.compare("-uid") == 0)
	{
		mType = Uid;
	}
	else if(s.compare("-gid") == 0)
	{
		mType = Gid;
	}
	else if(s.compare("-atime") == 0)
	{
		mType = Atime;
	}
	else if(s.compare("-mtime") == 0)
	{
		mType = Mtime;
	}
	else if(s.compare("-ctime") == 0)
	{
		mType = Ctime;
	}
	else if(s.compare("-perm") == 0)
	{
		mType = Perm;
	}
	// the user input did not match any of the types
	else
	{
		flag = false;
		cerr<<"No such option: "<<s<<endl;
	}

	return flag;
}


/*
 * Name :        SearchTask::CheckValidity()
 * Description : check the target read in from command line
 * Parameters :  void
 * Returns :     true if the target format is correct
 */
bool SearchTask::CheckValidity()
{
	bool flag = false;

	// check the target format according to the type
	switch(mType)
	{
	case Name:
		flag = true;
		break;
	case Size:
	case Uid:
	case Gid:
		flag = CheckNumericalTarget();
		break;
	case Atime:
	case Mtime:
	case Ctime:
		flag = CheckTimeTarget();
		break;
	case Perm:
		flag = CheckPermissionTarget();
		break;
	}

	return flag;
}

/*
 * Name :        SearchTask::GetIntTarget()
 * Description : get the integer value of the target
 * Parameters :  void
 * Returns :     int
 */
int SearchTask::GetIntTarget() const
{
	int value = atoi(mTarget.c_str());
	return value;
}

/*
 * Name :        SearchTask::GetLongTarget()
 * Description : get the long integer value of the target
 * Parameters :  void
 * Returns :     long int
 */
long int SearchTask::GetLongTarget() const
{
	long int value = atol(mTarget.c_str());
	return value;
}

/*
 * Name :        SearchTask::CheckNumericalTarget()
 * Description : check the correctness of the numerical target (size, uid, gid)
 * Parameters :  void
 * Returns :     true if the format does comply with the specification
 */
bool SearchTask::CheckNumericalTarget()
{
	bool flag = true;

	for(unsigned int i = 0; i<mTarget.size(); i++)
	{
		int currentInt = mTarget[i];
		// check if all the character belongs to '0' to '9'
		if(currentInt < 48 || currentInt > 57)
		{
			cerr<<"Invalid Character in SIZE/UID/GID parameter"<<endl;
			cerr<<"Correct FORMAT: decimal numbers"<<endl;
			flag = false;
			break;
		}
	}

	return flag;
}

/*
 * Name :        SearchTask::CheckTimeTarget()
 * Description : check the correctness of the time target
 * Parameters :  void
 * Returns :     true if the format does comply with the specification
 */
bool SearchTask::CheckTimeTarget()
{
	bool flag = true;

	// check if the length of the time-date is correct
	if(mTarget.size() != DateCharNumber)
	{
		flag = false;
	}

	// check if the format of the time-date is correct
	else
	{
		//**************************************************************************
		// check time portion
		//**************************************************************************

		// hour
		const char h1 = mTarget[0];
		const char h2 = mTarget[1];
		// minute
		const char m1 = mTarget[3];
		const char m2 = mTarget[4];
		// second
		const char s1 = mTarget[6];
		const char s2 = mTarget[7];

		if(h1<'0' || h1>'2' || h2<'0' || h2>'9' || m1<'0' || m1>'5' || m2<'0' || m2>'9'
			|| s1<'0' || s1>'5' || s2<'0' || s2>'9')
		{
			flag = false;
		}

		//**************************************************************************
		// check date portion
		//**************************************************************************

		// month
		const char mm1 = mTarget[9];
		const char mm2 = mTarget[10];

		// day
		const char dd1 = mTarget[12];
		const char dd2 = mTarget[13];

		// year
		const char yy1 = mTarget[15];
		const char yy2 = mTarget[16];

		if(mm1 <'0' || mm1 >'1' || mm2<'0' || mm2 >'9' || dd1<'0' || dd1>'3'
			|| dd2<'0' || dd2>'9' || yy1<'0' || yy1 >'9' || yy2<'0' || yy2>'9')
		{
			flag = false;
		}

		//**************************************************************************
		// check deliminators
		//**************************************************************************
		const char c1 = mTarget[2];
		const char c2 = mTarget[5];
		const char dash = mTarget[8];
		const char sh1 = mTarget[11];
		const char sh2 = mTarget[14];
		if(c1!= ':' || c2!=':' || dash!='-' || sh1!='/' || sh2!='/')
		{
			flag = false;
		}
	}

	// if something wrong, print out error message
	if(!flag)
	{
		cerr<<"Invalid time-date string"<<endl;
		cerr<<"Correct FORMAT: hh:mm:ss-mm/dd/yy"<<endl;
	}
	return flag;
}

/*
 * Name :        SearchTask::CheckPermissionTarget()
 * Description : check the correctness of the permission target
 * Parameters :  void
 * Returns :     true if the format does comply with the specification
 */
bool SearchTask::CheckPermissionTarget()
{
	bool flag = true;

	// check if the length of the permission is correct
	if(mTarget.size() != PermissionCharNumber)
	{
		flag = false;
	}
	else
	{
		for(unsigned int i = 0; i<mTarget.size(); i++)
		{
			int currentInt = mTarget[i];
			// check if all the character belongs to '0' to '7'
			if(currentInt < 48 || currentInt > 55)
			{
				flag = false;
				break;
			}
		}
	}

	if(!flag)
	{
		cerr<<"Invalid Character in Permission parameter"<<endl;
		cerr<<"Correct FORMAT: 3 octal numbers"<<endl;
	}

	return flag;
}





