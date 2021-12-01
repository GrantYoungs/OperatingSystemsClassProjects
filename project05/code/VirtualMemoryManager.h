/*
 * File    : VirtualMemoryManager.h
 *
 * Header for the VirtualMemoryManager class for Project5.
 *
 * Class   : VirtualMemoryManager.h
 * Impl    : VirtualMemoryManager.cpp
 * Created : Feb 26, 2012
 * Author  : CSE410 Spring 2012
 *
 * Notes   : Feel free to make any changes
 */

#ifndef VIRTUALMEMORYMANAGER_H_
#define VIRTUALMEMORYMANAGER_H_
#include <string>
#include <vector>

class Reference;

class VirtualMemoryManager
{
public:

	VirtualMemoryManager();
	virtual ~VirtualMemoryManager();

	// Setters
	void SetAlgorithm(std::string &s){}
	void SetTotalReferenceNumber(const int n) {mTotalReferenceNumber = n;}
	void SetProcessNumber(const int n) {mProcessNumber = n;}
	void SetTLBSize(const int n) {mTLBSize = n;}
	void SetPageOffset(const int n) {mPageOffset = n;}
	void SetMemoryFrameNumber(const int n ){mMemoryFrameNumber = n;}

	// Getters
	int GetTotalReferenceNumber() const {return mTotalReferenceNumber;}
	int GetProcessNumber() const {return mProcessNumber;}
	int GetTLBSize() const {return mTLBSize;}
	int GetPageOffset() const {return mPageOffset;}
	int GetMemoryFrameNumber() const {return mMemoryFrameNumber;}

	void Start();

private:

	// For retrieving references
	void RetrieveReferences();

	void processReferences();

	int mProcessNumber;
	int mTotalReferenceNumber;
	int mTLBSize;
	int mPageOffset;
	int mMemoryFrameNumber;

	std::vector<Reference*>mReferences;
};

#endif /* VIRTUALMEMORYMANAGER_H_ */
