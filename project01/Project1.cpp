//============================================================================
// Name        : Project1.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : CSE410 Project1 Template
//============================================================================

#include "SearchTask.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <string>
using namespace std;

// the recursive option string
const string RecursiveToken = "-R";

/*
 * Name :        ProcessArguments()
 * Description : the function used for reading in the command line input
 *				 then processes each <option> and <target>
 *				 and stores it in a vector
 * Parameters :  tasks - passed by reference, used for storing the processed
 * 				 		 user input (in the form of SearchTask objects)
 * 				 argv  - command line input
 * 				 argc  - number of tokens from command line input
 * Returns :     true if no errors happened during the whole procedure
 * Note	   :     Do not modify this function
 */
bool ProcessArguments(vector<SearchTask> &tasks, const char* argv[], const int argc);

/*
 * Name :        Find()
 * Description : This function displays the results of searching the directory at
 *				 "searchPath" for files that meet all of the search criteria in the
 *				 tasks (a vector containing SearchTasks).
 *				 All output goes to the standard output.
 * Parameters :  tasks - passed by reference, used for storing the processed
 * 				 		 user input (in the form of SearchTask objects)
 * 				 searchPath - the path retrieved from command line input (from user)
 * 				 isRecursive - this is used for Extra Credits (optional)
 * Returns :     true if no errors happened during the whole procedure
 */
bool Find(vector<SearchTask> &tasks, const string &searchPath, const bool isRecursive);

/*
 * Name :        main()
 * Description : the main function of the program. Its main purpose is to
 *	 	 	 	 process the argument list and then call the search function
 *	 	 	 	 at the end. In particular, it figures out which search
 *	 	 	 	 parameters have been requested and also looks for errors
 *	 	 	 	 made in the argument listing.
 * Parameters :  command line input for search path, option and target
 * Returns :     0 if the program runs successfully.
 *               1 if error occurs.
 */
int main(int argc, const char* argv[])
{
	// check for arguments
	int argCount = argc;

	// check if we need to find recursively
	bool isRecursive = false;

	// we check if the last token is "-R" which means
	// we want to find the target recursively
	string lastToken = argv[argc-1];
	if(lastToken.compare(RecursiveToken) == 0)
	{
		argCount -=1;
		isRecursive = true;
	}

	// check if the number of command line input is incorrect
	if(argc <= 3 || (argCount)%2 != 0)
	{
		cerr<<"Check the number of arguments!"<<endl;
		cerr<<"Usage: project1 <search path> <option> <target> ..."<<endl;
		return 1;
	}

	// store the search path
	const string SearchPath = argv[1];

	// vector for storing all the SearchTasks from user
	vector<SearchTask>allTasks;

	// start processing user input
	bool flag = ProcessArguments(allTasks, argv, argCount);

	// return 1 if there are errors while doing ProcessArguments
	// otherwise we start Find()
	if(flag) flag = Find(allTasks, SearchPath, isRecursive);
	else return 1;

	// return 0 if everything is done successfully
	// return 1 if something goes wrong
	if(flag) return 0;
	else return 1;

}

// This is the function used for processing the user input
// you do not need to make any change to it.
bool ProcessArguments(vector<SearchTask> &tasks, const char* argv[], const int argc)
{
	bool flag = true;

	for(int i =2; i<argc; i+=2)
	{
		SearchTask task;
		flag = task.ProcessOptionTarget(argv[i],argv[i+1]);
		if(!flag) break;
		else
		{
			tasks.push_back(task);
		}
	}

	return flag;
}

// you have to implement the Find() function
// this is just a template please implement it.
bool Find(vector<SearchTask> &tasks, const string &searchPath, const bool isRecursive)
{
	// the boolean value that will be returned
	bool flag = true;

	// open the directory corresponding to the searchPath

	// todo

	// read the directory

	// todo

	// for each entry of the directory
	// we check if the current entry
	// satisfies any of our search tasks
	for(unsigned int i = 0; i<tasks.size();i++)
	{
		SearchTask::TaskType currentType = tasks[i].GetType();
		SearchTask currentTask = tasks[i];

		switch(currentType)
		{
		// If the current task is of type Name
		case SearchTask::Name:
			//Replace this with your own code
			cout<<currentTask.GetStringTarget()<<endl;
			break;
		// If the current task is of type Size
		case SearchTask::Size:
			//Replace this with your own code
			cout<<currentTask.GetLongTarget()<<endl;
			break;
		// If the current task is of type Uid
		case SearchTask::Uid:
			//Replace this with your own code
			cout<<currentTask.GetIntTarget()<<endl;
			break;
		// If the current task is of type Gid
		case SearchTask::Gid:
			//Replace this with your own code
			cout<<currentTask.GetIntTarget()<<endl;
			break;
		// If the current task is of type Atime
		case SearchTask::Atime:
			//Replace this with your own code
			cout<<currentTask.GetStringTarget()<<endl;
			break;
		// If the current task is of type Mtime
		case SearchTask::Mtime:
			//Replace this with your own code
			cout<<currentTask.GetStringTarget()<<endl;
			break;
		// If the current task is of type Ctime
		case SearchTask::Ctime:
			//Replace this with your own code
			cout<<currentTask.GetStringTarget()<<endl;
			break;
		// If the current task is of type Perm
		case SearchTask::Perm:
			//Replace this with your own code
			cout<<currentTask.GetIntTarget()<<endl;
			break;
		}
	}

	// Optional for Extra Credits
	// find the target recursively
	if(isRecursive)
	{
		// todo
	}

	return flag;
}
