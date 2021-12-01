/*
 * File: ReferenceGenerator.cpp
 *
 * The class for generating references
 *
 * Header: ReferenceGenerator.h
 * Author: CSE410 Spring 2012
 *
 */

#include "VirtualMemoryManager.h"
#include "ReferenceGenerator.h"
#include "Reference.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

// to make sure there must be some collisions happened,
// we limit our page # always between 0-39
const int PageFactor = 40;

/*
 * Name :        ReferenceGenerator::ReferenceGenerator()
 * Description : constructor
 * Note :
 */
ReferenceGenerator::ReferenceGenerator(const int p, const int r, VirtualMemoryManager* m)
{
	mProcessNumber = p;
	mTotalReferenceNumber = r;
	mRemainingCount = 0;
	mMemoryManager = m;

	Initialize();
}

/*
 * Name :        ReferenceGenerator::~ReferenceGenerator()
 * Description : Default destructor
 * Note :
 */
ReferenceGenerator::~ReferenceGenerator()
{
}

/*
 * Name :        ReferenceGenerator::Initialize()
 * Description : doing some initial steps before generating
 * 				 references
 * Note :
 */
void ReferenceGenerator::Initialize()
{
	// set the remaining count to the total reference number
	mRemainingCount = mTotalReferenceNumber;

	const int processNum = mProcessNumber;
	const int totalRN = mTotalReferenceNumber;
	for(int i = 0; i< processNum; i++)
	{
		int refNum = totalRN/processNum;
		if(i == (processNum-1))
		{
			refNum = totalRN - (totalRN/processNum)*i;
		}

		Record *r = new Record;
		r->processID = i;
		r->pageNumberDivider = PageFactor;

		for(int j = 0; j<refNum; j++)
		{
			(r->pages).push_back(j);
		}
		mRecords.push_back(r);
	}
}

/*
 * Name :        ReferenceGenerator::GenerateReference()
 * Description : generate one reference at a time (per invocation)
 * Note :		 return a pointer to a newly generated reference on success
 * 				 return NULL when nothing to be generated (done)
 */
Reference *ReferenceGenerator::GenerateReference()
{
	Reference *ref = NULL;

	if(mRemainingCount > 0)
	{
		ref = new Reference;

		// initialize random seed
		srand(mRemainingCount);

		// pick up a process randomly
		const int recordIndex = rand() % (mRecords.size());

		Record* record = mRecords[recordIndex];

		// set process id
		ref->SetProcessID(record->processID);

		// random page number
		srand(mRemainingCount+1);
		const int pageIndex = rand() % (record->pages).size();
		int pageNumber = ((record->pages)[pageIndex])%(record->pageNumberDivider);
		(record->pages).erase((record->pages).begin()+pageIndex);

		// set virtual address
		ref->SetVirtualAddress(pageNumber, mMemoryManager->GetPageOffset());

		// random operation
		Reference::Operation op = ((pageNumber%4) >= 2)?(Reference::Read):(Reference::Write);

		// set operation type
		ref->SetOperation(op);

		// remove the record if it is done
		if((record->pages).size() == 0)
		{
			delete mRecords[recordIndex];
			mRecords.erase(mRecords.begin()+recordIndex);
		}
		mRemainingCount--;
	}
	return ref;
}
