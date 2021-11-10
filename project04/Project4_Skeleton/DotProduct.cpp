/*
 * File: DotProduct.h.cpp
 *
 * The class for dot product operation
 *
 * Header:  DotProduct.h
 * Author: CSE410 Spring 2012
 *
 */

#include "DotProduct.h"
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>
using namespace std;

// constant values
const unsigned int VectorNumber = 2;
const unsigned int NumberOfProcess = 2;
const unsigned int NumberOfThread = 2;
const int BASE = 100;
const int GenerateLimit = 1;

/*
 * Name :        DotProduct::DotProduct()
 * Description : Default constructor
 */
DotProduct::DotProduct()
{
	// initialize the member variables
	mMode = Normal;
	mProduct = NULL;
	mShmSemId = 0;
	mSem = NULL;
	mThreadInfos = NULL;
}


/*
 * Name :        DotProduct::~DotProduct()
 * Description : Default destructor
 * Note :        You have to implement this function
 */
DotProduct::~DotProduct()
{
	if(mMode != MP)
	{
		// clean the object created by "new"
		delete mProduct;

		if(mMode == MT)
		{
			// do not forget to destroy mutex
			// by using pthread_mutex_destroy()
			// and also delete any other objects
			// allocated with "new" if any

		}
	}
	else
	{
		// to deallocate shared memory:
		// first detach the shared memory by shmdt()
		// remove shared memory by shmctl()

		// to deallocate a semaphore allocated with a shared memory,
		// first destroy the semaphore by sem_destroy()
		// the follow the above steps.

		// also do not forget to delete any other
		// objects allocated with "new", if any
	}
}

/*
 * Name :        DotProduct::Start()
 * Description : start the dot product operation
 *
 */
void DotProduct::Start()
{
	if(CheckVectors())
	{
		switch(mMode)
		{
		case Normal:
			NormalDot();
			break;
		case MP:
			MultiProcessDot();
			break;
		case MT:
			MultiThreadDot();
			break;
		}
		// print on the screen
		Print();
	}
	else
	{
		cerr<<"The numbers of variables in each vector are not equal";
	}
}

/*
 * Name :        DotProduct::Initialize()
 * Description : Initialize the vectors and invoke the readinfile() function
 *
 */
void DotProduct::Initialize(const int argc, const char** argv)
{
	// a vector of vectors for storing the values read in from the file
	// currently, there are two vectors: mVectors[0] and mVectprs[1]
	for(unsigned int i = 0; i< VectorNumber; i++)
	{
		mVectors.push_back( vector<int>() );
	}

	// set the operation mode: Normal, multi-process, multi-thread
	SetMode(argv[2]);

	// Generate values for vectors
	GenerateValues(argv[1]);

	// we allocate a normal memory to mProduct
	// if the current mode is not MP (Multiprocess)
	if(mMode != MP)
	{
		mProduct = new int(0);
	}
	else
	{
		// initialize the mProduct in the following function
		MultiProcessInitialize();
	}
}

/*
 * Name :        DotProduct::SetMode()
 * Description : based on the command line input, we set the operation mode
 *
 */
void DotProduct::SetMode(const char* m)
{
	char mode = m[1];
	switch(mode)
	{
	case 'N':
		mMode = Normal;
		break;
	case 'P':
		mMode = MP;
		break;
	case 'T':
		mMode = MT;
		break;
	}
}

/*
 * Name :        DotProduct::SetMode()
 * Description : based on the command line input, we set the operation mode
 *
 */
bool DotProduct::CheckVectors()
{
	bool flag = true;
	unsigned int size = 0;
	for(unsigned int i = 0; i<VectorNumber; i++)
	{
		if(i == 0)
		{
			size = mVectors[i].size();
		}
		else
		{
			unsigned int currSize = mVectors[i].size();
			if(size != currSize)
			{
				flag = false;
				break;
			}
		}
	}

	if(flag)
	{
		mVectorSize = size;
	}
	return flag;
}

/*
 * Name :        DotProduct::GenerateValues()
 * Description : generate values for vectors
 * Parameters :  char*
 * Returns :     void
 * Note :        You have to implement this function
 */
void DotProduct::GenerateValues(const char *n)
{
	// number of values in each vector
	mNumberOfValuesPerVector = atoi(n);

	// total # of values the producer
	// has to generate and store in vectors
	mTotalNumberOfValues = mNumberOfValuesPerVector*2;

	// initialize mutex and conditional variables
	// by pthread_cond_init() and pthread_mutex_init()

	// we will have one producer and two consumers
	// create new threads by pthread_create()

	// join all threads by pthread_join()

	// destroy mutex and conditional variables
}

/*
 * Name :        DotProduct::ProducerEntry()
 * Description : this is the function used in pthread_create()
 * 				 serves as an entry point for the thread of producer
 * Parameters :  void
 * Returns :     void
 */
void *DotProduct::ProducerEntry(void *arg)
{
	DotProduct* obj = (DotProduct*)arg;
	obj->Producer();
	return NULL;

}

/*
 * Name :        DotProduct::ConsumerEntry()
 * Description : this is the function used in pthread_create()
 * 				 serves as an entry point for the thread of consumer
 * Parameters :  void
 * Returns :     void
 */
void *DotProduct::ConsumerEntry(void* arg)
{
	ThreadInfo* info = (ThreadInfo*)arg;
	(info->obj)->Consumer(info->Index);
	return NULL;
}

/*
 * Name :        DotProduct::Producer()
 * Description : the function that generates values
 * Parameters :  void
 * Returns :     void
 * Note :        You have to implement this function
 */
void DotProduct::Producer()
{
	// you have to use:
	// pthread_mutex_lock(), pthread_mutex_unlock()
	// pthread_cond_wait(), pthread_cond_signal()
	// on mutex (mMutex1) and conditional variables (mNotEmpty,mEmpty)
	// at the end, you would need pthread_exit()
}

/*
 * Name :        DotProduct::Consumer()
 * Description : the function that consumes values
 * Parameters :  char*
 * Returns :     void
 * Note :        You have to implement this function
 */
void DotProduct::Consumer(const unsigned int index)
{
	// you have to use:
	// pthread_mutex_lock(), pthread_mutex_unlock()
	// pthread_cond_wait(), pthread_cond_signal()
	// on mutex (mMutex1) and conditional variables (mNotEmpty,mEmpty)
	// at the end, you would need pthread_exit()
}

/*
 * Name :        DotProduct::NormalDot()
 * Description : do dot product operation in single process/thread mode
 * Parameters :  void
 * Returns :     void
 * Note :        You have to implement this function
 */
void DotProduct::NormalDot()
{

}

/*
 * Name :        DotProduct::MPInitialize()
 * Description : initialize semaphore and create shared memory
 * Parameters :  void
 * Returns :     void
 * Note :        You have to implement this function
 */
void DotProduct::MultiProcessInitialize()
{

	// you have to allocate a shared memory segment for mProduct and mSem
	// for sharing it between processes and protecting the critical section
	// also, you have to initialize the semaphore after allocating shared memory
	// functions needed here: shmget(), shmat(), sem_init()
}

/*
 * Name :        DotProduct::MPDot()
 * Description : do fork and invoke MTDotOperation() here
 * Parameters :  void
 * Returns :     void
 * Note :        You have to implement this function
 */
void DotProduct::MultiProcessDot()
{

}

/*
 * Name :        DotProduct::ProcessDotOperation()
 * Description : where dot product operation is done in each process
 * Parameters :  void
 * Returns :     void
 * Note :        You have to implement this function
 */
void DotProduct::ProcessDotOperation(unsigned int startIndex, unsigned int endIndex)
{
	// do dot product operation and
	// store the result to mProduct
	// make sure that you use semaphore to
	// protect the critical section
	// by using sem_wait() & sem_post()
}

/*
 * Name :        DotProduct::MultiThreadDot()
 * Description : initialize mutex and create threads here
 * Parameters :  void
 * Returns :     void
 * Note :        You have to implement this function
 */
void DotProduct::MultiThreadDot()
{
	// first, initialize the mutex with pthread_mutex_init()
	// use pthread_create() to create threads
	// and, then, do dot product operation
	// at the end, don't forget to use pthread_join()
	// for waiting for the termination of all threads created
}


/*
 * Name :        DotProduct::ThreadEntry()
 * Description : this is the function used in pthread_create()
 * 				 serves as an entry point for the thread
 * Parameters :  void
 * Returns :     void
 * Note :        You have to implement this function
 */
void *DotProduct::ThreadEntry(void *arg)
{
	// invoke the ThreadDotOperation() here
}


/*
 * Name :        DotProduct::ThreadDotOperation()
 * Description : where dot product operation is done in each thread
 * Parameters :  void
 * Returns :     void
 * Note :        You have to implement this function
 */
void DotProduct::ThreadDotOperation(unsigned int startIndex, unsigned int endIndex)
{
	// do dot product operation and
	// store the result to mProduct
	// make sure that you use mutex to
	// protect the critical section
	// by using pthread_mutex_lock() and pthread_mutex_unlock()
	// at the end of the function, use pthread_exit() to terminate the thread
}

/*
 * Name :        DotProduct::Print()
 * Description : print the content of vectors and result on the screen
 * Parameters :  void
 * Returns :     void
 * Note :        You have to implement this function
 */
void DotProduct::Print()
{
}
