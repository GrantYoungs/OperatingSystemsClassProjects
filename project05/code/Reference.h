/*
 * File    : Reference.h
 *
 * Header for the Reference class for Project5.
 *
 * Class   : Reference.h
 * Impl    : Reference.cpp
 * Created : Feb 26, 2012
 * Author  : CSE410 Spring 2012
 *
 * Notes   : Feel free to make any changes
 */

#ifndef REFERENCE_H_
#define REFERENCE_H_

class Reference
{
public:
	enum Operation {Read = 0, Write = 1};
	Reference();
	virtual ~Reference();

	// Setters
	void SetOperation(const Operation p) {mOperation = p;}
	void SetProcessID(const int i){mProcessID = i;}
	void SetVirtualAddress(const int i, const int pageOffset);


	// Getters
	int GetProcessID() const {return mProcessID;}
	int GetVirtualAddress() const {return mVirtualAddress;}
	Operation GetOperation() const {return mOperation;}

	// print all the information in each reference (not required for the project)
	void Print();

private:

	// process ID
	int mProcessID;

	// address (pageNumber with offset)
	int mVirtualAddress;

	// operations (read or write)
	Operation mOperation;

};

#endif /* REFERENCE_H_ */
