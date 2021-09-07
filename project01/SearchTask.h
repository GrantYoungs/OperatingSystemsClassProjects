/*
 * File    : SearchTask.h
 *
 * Header for the SearchTask class for Project1.
 *
 * Class   : SearchTask.h
 * Impl    : SearchTask.cpp
 * Created : Jan 02, 2012
 * Author  : CSE410 Spring 2012
 *
 * Notes   : Do not modify this class
 */

#ifndef SEARCHTASK_H_
#define SEARCHTASK_H_
#include <string>
class SearchTask
{
public:
	// TaskType to represent the option input from user
	enum TaskType{Name, Size, Uid, Gid, Atime, Mtime, Ctime, Perm};

	SearchTask();
	virtual ~SearchTask();

	// Getters
	TaskType GetType() const {return mType;}
	std::string GetStringTarget() const {return mTarget;}
	long int GetLongTarget() const;
	int GetIntTarget() const;

	// Function for initializing the SearchTask object
	bool ProcessOptionTarget(const std::string option, const std::string target);

private:
	// private functions used by SearchTask class only
	bool SetType(const std::string s);
	void SetStringTarget(const std::string s){mTarget = s;}
	bool CheckValidity();
	bool CheckNumericalTarget();
	bool CheckTimeTarget();
	bool CheckPermissionTarget();

	// private member variables
	TaskType mType;
	std::string mTarget;
};

#endif /* SEARCHTASK_H_ */
