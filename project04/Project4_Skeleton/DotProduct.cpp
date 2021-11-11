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
#include <random>
#include <pthread.h>
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
			// cout << "Uncomment NormalDot" << endl;
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
	// currently, there are two vectors: mVectors[0] and mVectors[1]
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
	// initialize mutex and conditional variables: 
	pthread_mutex_init(&mMutex1, NULL);
	pthread_cond_init(&mNotEmpty, NULL);
	pthread_cond_init(&mEmpty, NULL);

	// we will have one producer and two consumers
	// create new threads by pthread_create()

	pthread_t producer_thread;
	int create = pthread_create(&producer_thread, NULL, ProducerEntry, (void *)this);
	if (create != 0)
	{
		cerr << "pthread create failed for producer thread" << endl;
	}

	// Create the consumer threads. Should be one consumer thread per number of vectors (by default, 2)
	pthread_t consumer_threads[VectorNumber];
	for (int i = 0; i < VectorNumber; i++)
	{
		ThreadInfo* threadInfo = new ThreadInfo();
		threadInfo->obj = this;
		threadInfo->Index = i; // Index in this case will refer to which vector in mVectors the thread will be adding values to.

		int consumer_create = pthread_create(&consumer_threads[i], NULL, ConsumerEntry, (void *) threadInfo);
		if (consumer_create != 0)
		{
			cerr << "pthread create failed for consumer thread" << endl;
		}
	}

	// join all threads by pthread_join()
	int join = pthread_join(producer_thread, NULL);
	if (join != 0)
	{
		cerr << "pthread join failed for producer thread" << endl;
	}

	for (int i = 0; i < VectorNumber; i++)
	{
		int consumer_join = pthread_join(consumer_threads[i], NULL);
		if (consumer_join != 0)
		{
			cerr << "pthread join failed for consumer thread" << endl;
		}
	}

	// destroy mutex and conditional variables
	pthread_mutex_destroy(&mMutex1);
	pthread_cond_destroy(&mNotEmpty);
	pthread_cond_destroy(&mEmpty);
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

	std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, BASE); // define the range

	for (int i = 0; i < mTotalNumberOfValues; i++)
	{
		// Get a lock on the mutex
		pthread_mutex_lock(&mMutex1);

		// While mGeneratedNumber vector still has numbers to be consumed,
		// wait for the consumer threads to signal that it is empty.
		while (mGeneratedNumber.size() != 0)
		{
			pthread_cond_wait(&mEmpty, &mMutex1);
		}

		/* CRITICAL SECTION */
		mGeneratedNumber.push_back(distr(gen));
		/* END CRITICAL SECTION */

		// Signal that mGeneratedNumber vector is not empty and has numbers to be consumed.
		// Then, release the lock on the mutex.
		pthread_cond_signal(&mNotEmpty);
		pthread_mutex_unlock(&mMutex1);
	}

	pthread_exit(NULL);
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

	for (int i = 0; i < mNumberOfValuesPerVector; i++)
	{
		pthread_mutex_lock(&mMutex1);

		while (mGeneratedNumber.size() == 0)
		{
			pthread_cond_wait(&mNotEmpty, &mMutex1);
		}
		
		/* CRITICAL SECTION */
		int num = mGeneratedNumber.at(mGeneratedNumber.size() - 1); // Grab the last number
		mGeneratedNumber.pop_back(); // Remove the last number
		mVectors.at(index).push_back(num);
		/* END CRITICAL SECTION */

		if (mGeneratedNumber.size() == 0)
		{
			pthread_cond_signal(&mEmpty);
		}

		pthread_mutex_unlock(&mMutex1);
	}

	pthread_exit(NULL);
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
	// Iterate through all the values across the vectors in mVectors
	for (int i = 0; i < mNumberOfValuesPerVector; i++)
	{
		int sum = 1;

		// Iterate through each vector in mVectors,
		// and multiply sum by the i-th element in that vector.
		for (const auto & vec : mVectors)
		{
			sum *= vec.at(i);
		}

		// Add that sum to the total mProduct
		*mProduct += sum;
	}
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
	return NULL; // Delete later
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
	int vectorNumber = 1;
	for (const auto & vec : mVectors)
	{
		cout << "Vector " << vectorNumber << "\t: ";
		PrintVector(vec);
		cout << endl;

		vectorNumber++;
	}

	// Print the dot product result
	cout << "Dot Product = " << *mProduct << endl;
}

/*
 * Name :        DotProduct::PrintVector()
 * Description : Print the contents of vector in a neat format
 * Parameters :  vector<int> vec -- the vector to print
 * Returns :     void
 */
void DotProduct::PrintVector(const vector<int> &vec)
{
	cout << "[";

	for (int i = 0; i < vec.size(); i++)
	{
		cout << vec.at(i);

		// If it's the last element, add the closing bracket
		if (i == vec.size() - 1)
		{
			cout << "]";
		}
		else
		{
			cout << ", ";
		}
	}
}
