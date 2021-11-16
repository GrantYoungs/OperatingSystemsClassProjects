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
#include <unistd.h>
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

		/* NOTE
			Grant -- mMutex1, mEmpty, and mNotEmpy destroyed at the end of DotProduct::GenerateValues()
		*/

		if(mMode == MT)
		{
			// do not forget to destroy mutex
			// by using pthread_mutex_destroy()
			// and also delete any other objects
			// allocated with "new" if any

			/* NOTE
				Grant -- mMutex2 destroyed at the end of DotProduct::MultiThreadDot()
			*/
		}
	}
	else
	{
		// to deallocate shared memory:
		// first detach the shared memory by shmdt()
		// remove shared memory by shmctl()
		shmdt(mProduct);
		shmctl(mShmProductId, IPC_RMID, NULL);
		delete mProduct;

		// to deallocate a semaphore allocated with a shared memory,
		// first destroy the semaphore by sem_destroy()
		// the follow the above steps.
		sem_destroy(mSem);
		shmdt(mSem);
		shmctl(mShmSemId, IPC_RMID, NULL);

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
	pthread_t consumer_threads[NumberOfThread];
	for (int i = 0; i < NumberOfThread; i++)
	{
		ThreadInfo* threadInfo = new ThreadInfo(); // Will be deleted at the end of ConsumerEntry()
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

	for (int i = 0; i < NumberOfThread; i++)
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

	// Clean up info after Consumer() is ran
	delete info;

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
	// Assign IDs for the shared memory for the product and the semaphore
	mShmProductId = shmget(IPC_PRIVATE, sizeof(unsigned int) * 1, IPC_CREAT | SHM_R | SHM_W);
	mShmSemId = shmget(IPC_PRIVATE, sizeof(unsigned int) * 1, IPC_CREAT | SHM_R | SHM_W);

	// Attach mProduct and mSem to shared memory using the unique keys
	mProduct = static_cast<int *>(shmat(mShmProductId, 0, 0));
	mSem = static_cast<sem_t *>(shmat(mShmSemId, 0, 0));

	// Now that mProduct is in shared memory, we can initialize it
	mProduct = new int(0);

	// Initialize the semaphore mSem
	int sem_init_ret = sem_init(mSem, 1, 1);
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
	int mid = mNumberOfValuesPerVector / NumberOfProcess;

	pid_t pID = fork();
	if (pID < 0)
	{
		cout << "DotProduct::MultiProcessDot fork() error" << endl;
	}
	else if (pID == 0)
	{
		// Child process
		ProcessDotOperation(0, mid - 1);
	}
	
	// Parent process. Complete the operation, then wait for the child process to finish.
	ProcessDotOperation(mid, mNumberOfValuesPerVector - 1);
	waitpid(pID, NULL, 0);
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
	for (int i = startIndex; i <= endIndex; i++)
	{
		int sum = 1;

		for (const auto & vec : mVectors)
		{
			sum *= vec.at(i);
		}

		// Add that sum to the total mProduct
		sem_wait(mSem);

		/* CRITICAL SECTION */
		*mProduct += sum;
		/* END CRITICAL SECTION */

		sem_post(mSem);
	}
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
	pthread_mutex_init(&mMutex2, NULL);

	int chunk = (mNumberOfValuesPerVector / NumberOfThread); // 5 items => 2
	int nextStartIndex = 0;

	pthread_t thread_entries[NumberOfThread];
	for (int i = 0; i < NumberOfThread; i++)
	{
		ThreadInfo* threadInfo = new ThreadInfo();
		threadInfo->obj = this;
		threadInfo->Index = i; // Probably won't even be used

		
		threadInfo->StartIndex = nextStartIndex;	// Starting index of the section of the vectors this thread will be processing

		// Time to figure out the end index
		// This section is used to evenly split the responsibilities of the threads, so that each thread
		// works through roughly the same length in the vector.
		if (i == NumberOfThread - 1)
		{
			threadInfo->EndIndex = mNumberOfValuesPerVector - 1; // End index will be the last element of the vector
		}
		else
		{
			// If the vectors have an even amount of numbers
			if (mNumberOfValuesPerVector % 2 == 0)
			{
				threadInfo->EndIndex = threadInfo->StartIndex + chunk - 1;
			}
			else
			{
				threadInfo->EndIndex = threadInfo->StartIndex + chunk;
			}
		}

		nextStartIndex = threadInfo->EndIndex + 1;

		// Create threads after initializing the ThreadInfo objects
		int thread_entry_create = pthread_create(&thread_entries[i], NULL, ThreadEntry, (void *) threadInfo);
		if (thread_entry_create != 0)
		{
			cerr << "pthread create failed for ThreadDotOperation thread" << endl;
		}
	}

	// Join the threads now
	for (int i = 0; i < NumberOfThread; i++)
	{
		int thread_entry_join = pthread_join(thread_entries[i], NULL);
		if (thread_entry_join != 0)
		{
			cerr << "pthread join failed for ThreadDotOperation thread" << endl;
		}
	}

	// Cleanup
	pthread_mutex_destroy(&mMutex2);
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
	ThreadInfo* info = (ThreadInfo*)arg;
	(info->obj)->ThreadDotOperation(info->StartIndex, info->EndIndex);

	// delete the info object that was generated in the function that called this
	delete info;

	return NULL;
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
	for (int i = startIndex; i <= endIndex; i++)
	{
		int sum = 1;

		// Iterate through each vector in mVectors,
		// and multiply sum by the i-th element in that vector.
		for (const auto & vec : mVectors)
		{
			sum *= vec.at(i);
		}

		// Add that sum to the total mProduct
		pthread_mutex_lock(&mMutex2);
		*mProduct += sum;
		pthread_mutex_unlock(&mMutex2);
	}

	pthread_exit(NULL);
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
