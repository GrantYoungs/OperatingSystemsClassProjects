/*
 * File    : ReferenceGenerator.h
 *
 * Header for the ReferenceGenerator class for Project5.
 *
 * Class   : ReferenceGenerator.h
 * Impl    : ReferenceGenerator.cpp
 * Created : Feb 26, 2012
 * Author  : CSE410 Spring 2012
 *
 * Notes   : Feel free to make any changes
 */

#ifndef REFERENCEGENERATOR_H_
#define REFERENCEGENERATOR_H_
#include <vector>
class Reference;
class VirtualMemoryManager;

class ReferenceGenerator
{
public:
	ReferenceGenerator(const int p, const int r, VirtualMemoryManager* m);
	virtual ~ReferenceGenerator();

	// Setters
	void SetProcessNumber(const int n) {mProcessNumber = n;}
	void SetTotalReferenceNumber(const int n) {mTotalReferenceNumber = n;}

	// Getters
	int GetProcessNumber() const {return mProcessNumber;}
	int GetPageNumberPerProcess(const int i) {return mPageNumberPerProcess[i];}

	// return null if we are done
	Reference* GenerateReference();

	// the struct Record used when generating references
	struct Record
	{
		// process ID, starts from 0
		int processID;
		int pageNumberDivider;
		//page number, starts from 0
		std::vector<int> pages;
	};

private:
	ReferenceGenerator();
	void Initialize();

	// # of process number used for generating references
	int mProcessNumber;

	// # of reference we have to generate
	int mTotalReferenceNumber;

	// # of reference remains to be generated
	int mRemainingCount;

	std::vector<int> mPageNumberPerProcess;
	std::vector<Record*> mRecords;

	VirtualMemoryManager* mMemoryManager;
};

#endif /* REFERENCEGENERATOR_H_ */
