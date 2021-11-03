/*
 * File: Process.cpp
 *
 * The class for the process
 *
 * Header:  Process.h
 * Author: CSE410 Spring 2012
 *
 */

#include "Process.h"
#include <iostream>
using namespace std;

/*
 * Name :        Process::Process()
 * Description : Default constructor
 */
Process::Process()
{
	mID = 0;
	mArrivalTime = 0;
	mServiceTime = 0;
	mRemainingTime = 0;
	mStartTime = 0;
	mFinishTime = 0;
	mIsDone = false;
	mIsScheduled = false;
	mIsReady = false;

}

/*
 * Name :        Process::~Process()
 * Description : Default destructor
 */
Process::~Process()
{

}

/*
 * Name :        Process::SetServiceTime()
 * Description : set the service time as well as initialize the
 * 			     remaining time for execution
 * Parameters :  string of the service time of the process
 * Returns:      void
 *
 */
void Process::SetServiceTime(const int time)
{
	mServiceTime = time;
	SetRemainingTime(GetServiceTime());
}

/*
 * Name :        Process::SetStartTime()
 * Description : set the start time as well as set mIsStarted to true
 * Parameters :  string of the start time of the process
 * Returns:      void
 *
 */
void Process::SetStartTime(const int time)
{
	mStartTime = time;
	SetIsStarted();
}

/*
 * Name :        Process::SetFinishTime()
 * Description : set the finish time as well as set mIsDone to true
 * Parameters :  string of the finish time of the process
 * Returns:      void
 *
 */
void Process::SetFinishTime(const int time)
{
	mFinishTime = time;
	SetIsDone();
}









