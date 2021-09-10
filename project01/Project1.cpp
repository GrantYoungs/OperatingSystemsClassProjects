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
 * Name:			PrintStat
 * Description :	Prints out the contents of a stat struct in a formatted manner.
 * Parameters :		The struct stat to print
 * Note :			Private function, to remove when done with development.
 */
void PrintStat(const struct stat & fileStat);

/*
 * Name :			GetFileStat()
 * Description : 	Retrieves information about a file in a stat struct.
 * Parameters :		The absolute path to the file (e.g. c/Users/Grant/test.txt).
 * Returns :		Pointer to struct stat containing the file's information.
 */
struct stat * GetFileStat(const string & absoluteFileName);

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

	DIR * directory;
	struct dirent * file;	// Pointer to represent each file in the directory

	// Read through the directory indicated by the search path
	if ((directory = opendir(searchPath.c_str())) != NULL) 
	{
		// Iterate through all the files in the directory
		while ((file = readdir(directory)) != NULL) 
		{
			string fileName = string(file->d_name);
			if (fileName != "." && fileName != "..")
			{
				fileNames.push_back(fileName);
			}

			/*
			string absoluteFileName = searchPath + "/" + string(file->d_name);

			// Retrieve information about the file, store it into result
			struct stat result;
			if (stat(absoluteFileName.c_str(), &result) == 0)
			{
				cout << absoluteFileName << endl;
				PrintStat(result);
			}
			
			cout << endl;
			*/
		}
	}

	closedir(directory);

	// for each entry of the directory
	// we check if the current entry
	// satisfies any of our search tasks

	for (const auto & fileName : fileNames)
	{
		// Retrieve the file's information
		const struct stat * fileInfo = GetFileStat(searchPath + "/" + fileName);
		if (fileInfo == nullptr)
		{
			// If the file's info could not be found, move onto the next file in the list.
			continue;
		}

		for (unsigned int i = 0; i < tasks.size(); i++)
		{
			SearchTask::TaskType currentType = tasks[i].GetType();
			SearchTask currentTask = tasks[i];

			switch(currentType)
			{
			// If the current task is of type Name
			case SearchTask::Name:
				if (fileName == currentTask.GetStringTarget())
				{
					cout << fileName << endl;
				}
				break;
			// If the current task is of type Size
			case SearchTask::Size:
				if (fileInfo->st_size == currentTask.GetLongTarget())
				{
					cout << fileName << endl;
				}
				break;
			// If the current task is of type Uid
			case SearchTask::Uid:
				if (fileInfo->st_uid == currentTask.GetIntTarget())
				{
					cout << fileName << endl;
				}
				break;
			// If the current task is of type Gid
			case SearchTask::Gid:
				if (fileInfo->st_gid == currentTask.GetIntTarget())
				{
					cout << fileName << endl;
				}
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
	}

	// Optional for Extra Credits
	// find the target recursively
	if(isRecursive)
	{
		// todo
	}

	return flag;
}

void PrintStat(const struct stat & fileStat)
{
	cout << "st_dev: " << fileStat.st_dev << endl;
	cout << "st_ino: " << fileStat.st_ino << endl;
	cout << "st_mode: " << fileStat.st_mode << endl;
	cout << "st_nlink: " << fileStat.st_nlink << endl;
	cout << "st_uid: " << fileStat.st_uid << endl;
	cout << "st_gid: " << fileStat.st_gid << endl;
	cout << "st_rdev: " << fileStat.st_rdev << endl;
	cout << "st_size: " << fileStat.st_size << endl;
	cout << "st_blksize: " << fileStat.st_blksize << endl;
	cout << "st_blocks: " << fileStat.st_blocks << endl;
	cout << "st_atime: " << fileStat.st_atim.tv_sec << endl;
	cout << "st_mtime: " << fileStat.st_mtim.tv_sec << endl;
	cout << "st_ctime: " << fileStat.st_ctim.tv_sec << endl;
	if (fileStat.st_mode & S_IRUSR)
	{
		cout << "read ";
	}
	if (fileStat.st_mode & S_IWUSR)
	{
		cout << "write ";
	}
	if (fileStat.st_mode & S_IXUSR)
	{
		cout << "execute ";
	}
	cout << endl;

	// S_IRGRP, S_IWGRP, S_IROTH, S_IWOTH, S_IRWXU -> Macros for checking group permissions and all users permissions
}

struct stat * GetFileStat(const string & absoluteFileName)
{
	struct stat fileInfo;
	return (stat(absoluteFileName.c_str(), &fileInfo) == 0) ? &fileInfo : nullptr;
}
