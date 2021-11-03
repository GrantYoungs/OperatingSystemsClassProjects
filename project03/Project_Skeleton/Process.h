/*
 * File    : Process.h
 *
 * Header for the Process class for Project3.
 *
 * Class   : Process.h
 * Impl    : Process.cpp
 * Created : Jan 17, 2012
 * Author  : CSE410 Spring 2012
 *
 * Notes   : Do NOT modify this class
 */

#ifndef PROCESS_H_
#define PROCESS_H_
#include <vector>

class Process
{
public:
	Process();
	virtual ~Process();

	// Getters
	int GetID() const {return mID;};
	int GetArrivalTime() const {return mArrivalTime;}
	int GetServiceTime() const {return mServiceTime;}
	int GetRemainingTime() const {return mRemainingTime;}
	int GetStartTime() const {return mStartTime;}
	int GetFinishTime() const {return mFinishTime;}
	int GetNumberOfTimeSlots() const {return mTimeSlots.size();}
	int GetTimeSlot(const int index){return mTimeSlots[index];}
	bool IsDone() const {return mIsDone;}
	bool IsScheduled() const {return mIsScheduled;}
	bool IsReady() const {return mIsReady;}

	// Setters
	void AddTimeSlot(const int time){mTimeSlots.push_back(time);}
	void DecreaseRemainingTime(){mRemainingTime--;}
	void SetArrivalTime(const int time){mArrivalTime = time;}
	void SetID(const int id){mID = id;};
	void SetIsReady(){mIsReady = true;}
	void SetServiceTime(const int time);
	void SetStartTime(const int time);
	void SetFinishTime(const int time);

private:
	void SetRemainingTime(int time){mRemainingTime = time;}
	void SetIsDone(){mIsDone = true;}
	void SetIsStarted(){mIsScheduled = true;}

	int mID;
	int mArrivalTime;
	int mServiceTime;
	int mRemainingTime;
	int mStartTime;
	int mFinishTime;

	// is the process done?
	bool mIsDone;

	// is the process ever scheduled?
	bool mIsScheduled;

	// the schedule is ready for being executed?
	// a process is ready if and only if the arrival time
	// is smaller or equal to the current time for scheduling
	bool mIsReady;

	// time slots allocated to the process
	std::vector<int> mTimeSlots;
};

#endif /* PROCESS_H_ */
