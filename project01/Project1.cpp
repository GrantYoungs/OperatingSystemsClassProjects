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
#include <stdio.h>
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
 * Name :			IsCorrectFilePermissions()
 * Description :	This function determines if a file's permissions, retrieved from struct stat.st_mode,
 * 					matches the targeted permissions.
 * Parameters :		filePermissions - the file to check's permissions, retrieved from struct stat.st_mode.
 * 					targetPermissions - the permissions to compare against, passed as an integer (e.g. 755)
 * Returns :		true if the filePermissions match the targetPermissions.
 */
bool IsCorrectFilePermissions(const mode_t filePermissions, const int targetPermissions);

/*
 * Name :			IsMatchingTime()
 * Description :	This function determine's if a file's access, modify, or last status change time, 
 * 					given in time_t, matches a target time, given as a string in the format %T-%D (HH:MM:SS-MM/DD/YY)
 * Parameters :		fileTime - the time the file was last accessed.
 * 					targetTime - the time to match against the fileTime.
 * Returns :		true if the fileTime matches the targetTime.
 */
bool IsMatchingTime(const time_t & fileTime, const string & targetTime);

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

	vector<string> fileNames;
	vector<string> directoryNames;

	DIR * directory;
	struct dirent * file;	// Pointer to represent each file in the directory

	// Read through the directory indicated by the search path
	if ((directory = opendir(searchPath.c_str())) != NULL) 
	{
		// Iterate through all the files in the directory
		while ((file = readdir(directory)) != NULL) 
		{
			string name = string(file->d_name);

			// If the file is a regular file
			if (file->d_type == DT_REG)
			{
				fileNames.push_back(name);
			}

			// If the file is a directory
			else if (file->d_type == DT_DIR
					&& name != "."
					&& name != "..")
			{
				directoryNames.push_back(name);
			}
		}
	}
	closedir(directory);

	for (const auto & fileName : fileNames)
	{
		string absoluteFileName = searchPath[searchPath.size() - 1] == '/' ?
					searchPath + fileName : searchPath + "/" + fileName;
		
		// Retrieve the file info. If stat() returns anything other than 0, it failed to get that file's info. Move on to the next file.
		struct stat fileInfo;
		if (stat(absoluteFileName.c_str(), &fileInfo) != 0)
		{
			continue;
		}

		// If the file matches multiple targets, it only needs to be printed once.
		bool printed = false;

		for (unsigned int i = 0; i < tasks.size(); i++)
		{
			// If the file has already been printed, no need to search through the rest of the tasks
			if (!printed)
			{
				SearchTask::TaskType currentType = tasks[i].GetType();
				SearchTask currentTask = tasks[i];

				switch(currentType)
				{
				// If the current task is of type Name
				case SearchTask::Name:
					if (fileName == currentTask.GetStringTarget())
					{
						cout << absoluteFileName << endl;
						printed = true;
					}
					break;
				// If the current task is of type Size
				case SearchTask::Size:
					if (fileInfo.st_size == currentTask.GetLongTarget())
					{
						cout << absoluteFileName << endl;
						printed = true;
					}
					break;
				// If the current task is of type Uid
				case SearchTask::Uid:
					if (fileInfo.st_uid == currentTask.GetIntTarget())
					{
						cout << absoluteFileName << endl;
						printed = true;
					}
					break;
				// If the current task is of type Gid
				case SearchTask::Gid:
					if (fileInfo.st_gid == currentTask.GetIntTarget())
					{
						cout << absoluteFileName << endl;
						printed = true;
					}
					break;
				// If the current task is of type Atime
				case SearchTask::Atime:
					if (IsMatchingTime(fileInfo.st_atim.tv_sec, currentTask.GetStringTarget()))
					{
						cout << absoluteFileName << endl;
						printed = true;
					}
					break;
				// If the current task is of type Mtime
				case SearchTask::Mtime:
					if (IsMatchingTime(fileInfo.st_mtim.tv_sec, currentTask.GetStringTarget()))
					{
						cout << absoluteFileName << endl;
						printed = true;
					}
					break;
				// If the current task is of type Ctime
				case SearchTask::Ctime:
					if (IsMatchingTime(fileInfo.st_ctim.tv_sec, currentTask.GetStringTarget()))
					{
						cout << absoluteFileName << endl;
						printed = true;
					}
					break;
				// If the current task is of type Perm
				case SearchTask::Perm:
					if (IsCorrectFilePermissions(fileInfo.st_mode, currentTask.GetIntTarget()))
					{
						cout << absoluteFileName << endl;
						printed = true;
					}
					break;
				}
			}
			else
			{
				break;
			}
		}
	}

	// Optional for Extra Credits
	// find the target recursively
	if(isRecursive)
	{
		for (const auto & directoryName : directoryNames)
		{
			string _searchPath = searchPath[searchPath.size() - 1] == '/' ?
					searchPath + directoryName : searchPath + "/" + directoryName;

			flag = Find(tasks, _searchPath, isRecursive);
		}
	}

	return flag;
}

bool IsCorrectFilePermissions(const mode_t filePermissions, const int targetPermissions)
{
	/*
	 * In the parameter filePermissions, the first 9 bits of the integer indicate the bits for file permissions.
	 * A file with permissions 755 would have the first 9 bits of the number:
	 * 		...1 1110 1101
	 * 		...r wxr- xr-x
	 * 
	 * The bits beyond the first 9 are irrelevant for this function.
	 */

	// First, separate the user, group, and all permissions from targetPermissions
	int userPermissions = targetPermissions / 100;
	int groupPermissions = (targetPermissions % 100) / 10;
	int allPermissions = (targetPermissions % 100) % 10;

	// Shift the bits to the left to their appropriate positions.
	userPermissions = userPermissions << 6;
	groupPermissions = groupPermissions << 3;
	
	// Bitwise OR the three permission groups to get the integer with the appropriate bits marked for user, group, and other read, write, execute
	int totalPermissions = userPermissions | groupPermissions;
	totalPermissions = totalPermissions | allPermissions;

	// To isolate the first 9 bits of filePermissions, create a mask and bitwise AND filePermissions with the mask.
	unsigned int mask = 0x1ff;
	int _filePermissions = filePermissions & mask;

	// If the two are equal, then the file has the same permissions that we are looking for.
	return _filePermissions == totalPermissions;
}

bool IsMatchingTime(const time_t & fileTime, const string & targetTime)
{
	struct tm timeDate;
	strptime(targetTime.c_str(), "%T-%D", &timeDate);
	time_t compareTime = mktime(&timeDate);
	
	return compareTime == fileTime;
}
