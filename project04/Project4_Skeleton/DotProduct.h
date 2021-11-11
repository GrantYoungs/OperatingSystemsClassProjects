/*
 * File    : DotProduct.h
 *
 * Header for the DotProduct class for Project4.
 *
 * Class   : DotProduct.h
 * Impl    : DotProduct.cpp
 * Created : Feb 17, 2012
 * Author  : CSE410 Spring 2012
 *
 * Notes   : You need to implement or add some functions in this class
 */

#ifndef DOTPRODUCT_H_
#define DOTPRODUCT_H_
#include <semaphore.h>
#include <pthread.h>
#include <vector>

class DotProduct
{
public:
	// mode of the operation
	// MP for multiple processes
	// MT for multiple threads
	enum Mode {Normal, MP, MT};

	DotProduct();
	virtual ~DotProduct();

	void Initialize(const int argc, const char** argv);

	// start the dot product operation
	void Start();

	// the structure might be useful for threads
	struct ThreadInfo
	{
		DotProduct* obj;
		unsigned int Index;
		unsigned int StartIndex;
		unsigned int EndIndex;
	};

private:

	void SetMode(const char* m);
	void Print();
	bool CheckVectors();

	// functions for generating values in the vectors
	void GenerateValues(const char* n);
	static void* ProducerEntry(void* arg);
	static void* ConsumerEntry(void* arg);
	// producer/consumer
	void Producer();
	void Consumer(const unsigned int index);

	/*
	 * functions for single process/thread operation
	 */
	void NormalDot();

	/*
	 * functions for multi-process operation
	 */
	void MultiProcessInitialize();
	// this is where to do fork()
	void MultiProcessDot();
	// this is where each process does "dot product"
	void ProcessDotOperation(unsigned int startIndex, unsigned int endIndex);

	/*
	 * functions for multithread operation
	 */
	// this is where to pthread_create()
	void MultiThreadDot();
	// the entry point of the thread
	static void* ThreadEntry(void* arg);
	// this is where each thread does "dot product"
	void ThreadDotOperation(unsigned int startIndex, unsigned int endIndex);

	/*
	 * Print a vector in a neat format.
	 */
	void PrintVector(const std::vector<int> &vec);

	// operation mode
	Mode mMode;

	/*
	 * for generating values for vectors
	 */
	// store the values generated by the producer
	std::vector<int> mGeneratedNumber;

	// # of values in each vector
	int mNumberOfValuesPerVector;

	// total # of values in the vectors
	int mTotalNumberOfValues;

	// mutex and conditional variables
	pthread_cond_t mNotEmpty;
	pthread_cond_t mEmpty;
	pthread_mutex_t mMutex1;

	// this might be useful when dealing with threads
	ThreadInfo* mThreadInfos;

	// number of items stored in the vector
	// normally, each vector will contain equal numbers of variables
	unsigned int mVectorSize;

	// store the dot product result
	int* mProduct;

	// vectors storing values read from the file
	std::vector< std::vector<int> > mVectors;

	/*
	 * for parallel processes
	 */
	// for storing the shared memory id's returned from shmget()
	unsigned int mShmProductId;
	unsigned int mShmSemId;

	// a pointer to semaphore
	sem_t* mSem;

	/*
	 * for parallel threads
	 */
	// mutex for protecting critical section
	pthread_mutex_t mMutex2;
};

#endif /* DOTPRODUCT_H_ */
