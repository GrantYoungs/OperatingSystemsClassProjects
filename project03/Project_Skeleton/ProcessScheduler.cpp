/*
 * File: ProcessScheduler.cpp
 *
 * The class for scheduling the processes read from input file
 *
 * Header:  ProcessScheduler.h
 * Author: CSE410 Spring 2012
 *
 */

#include "ProcessScheduler.h"
#include "Process.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <list>
using namespace std;

// options
const string sFCFS = "-FCFS";
const string sSPN = "-SPN";
const string sSRT = "-SRT";
const string sRR = "-RR";

// sort processes according to the ServiceTime increasingly
bool CompareServiceTime(Process* p1, Process* p2)
{
	if((p1->GetServiceTime()) == (p2->GetServiceTime()))
	{
		return ((p1->GetID()) < (p2->GetID()));
	}
	else
	{
		return ((p1->GetServiceTime()) < (p2->GetServiceTime()));
	}
}

// sort processes according to the Process ID increasingly
bool CompareProcessID(Process* p1, Process* p2)
{
	return ((p1->GetID()) < (p2->GetID()));
}

// sort processes according to the RemainingTime increasingly
bool CompareRemainingTime(Process* p1, Process* p2)
{
	if((p1->GetRemainingTime()) == (p2->GetRemainingTime()))
	{
		return ((p1->GetID()) < (p2->GetID()));
	}
	else
	{
		return ((p1->GetRemainingTime()) < (p2->GetRemainingTime()));
	}
}

// Sort processes according to their arriving time, increasingly
bool CompareArrivalTime(Process * p1, Process * p2)
{
	if (p1->GetArrivalTime() == p2->GetArrivalTime())
	{
		return p1->GetID() < p2->GetID();
	}
	else
	{
		return p1->GetArrivalTime() < p2->GetArrivalTime();
	}
}

bool CompareForSPN(Process * p1, Process * p2)
{
	if (p1->GetArrivalTime() == p2->GetArrivalTime())
	{
		if (p1->GetServiceTime() == p2->GetServiceTime())
		{
			return p1->GetID() < p2->GetID();
		}
		else
		{
			return p1->GetServiceTime() < p2->GetServiceTime();
		}
	}
	else
	{
		return p1->GetArrivalTime() < p2->GetArrivalTime();
	}
}

bool TimeSlotSorter(pair<int, int> & p1, pair<int, int> & p2)
{
	return p1.first < p2.first;
}

/*
 * Name :        ProcessScheduler::ProcessScheduler()
 * Description : Constructor that takes a string (command line input) and
 *  			 an int for number of arguments as input
 */
ProcessScheduler::ProcessScheduler(const int argc, const char** args)
{
	// get file names
	mInputFileName = args[1];
	mOutputFileName = args[2];

	// get the option
	// initialize the option to NA (not available)
	mOption = NA;
	string option = args[3];
	SetOption(option);

	// if there is an argument, we store it
	if(argc > 4)
	{
		mQuantum = args[4];
	}

	// read in all the information from the file
	ReadInputFile(mInputFileName);
}

/*
 * Name :        ProcessScheduler::~ProcessScheduler()
 * Description : Default destructor
 */
ProcessScheduler::~ProcessScheduler()
{
	// clean up after ourselves
	for(unsigned int i = 0; i<mProcesses.size(); i++)
	{
		delete mProcesses[i];
	}
}

/*
 * Name :        ProcessScheduler::ReadInputFile()
 * Description : read in the process information
 * Parameters :  string of the file name
 * Returns:      void
 *
 */
void ProcessScheduler::ReadInputFile(const std::string file)
{
	ifstream inFile;

	// open the file
	inFile.open(file.c_str());

	// if the file is opened successfully
	if(inFile.is_open())
	{
		// keep reading the data from the file
		while(inFile.good())
		{
			// aTime for arrival time
			// sTime for service time
			string aTime, sTime;

			// get the data from the file
			inFile >> aTime;
			inFile >> sTime;

			if(inFile.eof())
			{
				break;
			}

			// add process to the process container
			AddProcess(aTime, sTime);
		}

		// set up ID's for processes
		if(mProcesses.size() > 0)
		{
			for(unsigned int i = 0; i<mProcesses.size(); i++)
			{
				mProcesses[i]->SetID(i+1);
			}
		}

		// close the file after using it
		inFile.close();
	}
	// if the file cannot be opened
	else
	{
		cout<<"Cannot open "<<mInputFileName<<endl;
		return;
	}
}

/*
 * Name :        ProcessScheduler::AddProcess()
 * Description : create and add a new process to the process container
 * Parameters :  arrival - the arrival time of the process
 * 				 service - the service time of the process
 * Returns:      void
 *
 */
void ProcessScheduler::AddProcess(const std::string arrival, const std::string service)
{
	// create and configure the new process according to the parameters passed in
	Process* p = new Process();
	p->SetArrivalTime(atoi(arrival.c_str()));
	p->SetServiceTime(atoi(service.c_str()));

	// store the process in the container
	mProcesses.push_back(p);
}

/*
 * Name :        ProcessScheduler::WriteToFile()
 * Description : write the scheduling result to the file
 * Parameters :  a file name for storing the result
 * Returns :     void
 * Note :        You have to implement this function
 */
void ProcessScheduler::WriteToFile()
{
	// Sort the processes by their ID's for a neat output
	SortByProcessID();

	ofstream outfile;
	outfile.open(mOutputFileName);

	for (const auto process : mProcesses)
	{
		int turnAroundTime = process->GetFinishTime() - process->GetArrivalTime();

		outfile << process->GetID() << " " 
				<< process->GetStartTime() << " " 
				<< process->GetFinishTime() << " "
				<< turnAroundTime << " "
				<< (float)turnAroundTime / (float)process->GetServiceTime() << endl;
	}

	outfile.close();
}

/*
 * Name :        ProcessScheduler::SetOption()
 * Description : set up the mOption value according to the user input
 * Parameters :  string of the option from the user
 * Returns:      true if everything works fine
 *
 */
bool ProcessScheduler::SetOption(const std::string option)
{
	bool flag = true;

	// assign a proper value to mOption
	if(option == sFCFS) mOption = FCFS;
	else if(option == sSPN) mOption = SPN;
	else if(option == sSRT) mOption = SRT;
	else if(option == sRR) mOption = RR;
	else
	{
		mOption = NA;
		flag = false;
	}
	return flag;
}

/*
 * Name :        ProcessScheduler::Execute()
 * Description : start scheduling according to the option
 * Parameters :  void
 * Returns :     void
 */
void ProcessScheduler::Execute()
{
	// start scheduling according to the option from command line input
	switch(mOption)
	{
	case FCFS:
		ExecuteFCFS();
		break;
	case SPN:
		ExecuteSPN();
		break;
	case SRT:
		ExecuteSRT();
		break;
	case RR:
		ExecuteRR();
		break;
	case NA:
		cerr<<"No such option!"<<endl;
		cout<<"Options: -FCFS, -SPN, -SRT, -RR"<<endl;
		break;
	}

	// after finishing scheduling, write the information into the output file
	WriteToFile();
}

/*
 * Name :        ProcessScheduler::ExecuteFCFS()
 * Description : schedule the process based on "first come first serve"
 * Parameters :  void
 * Returns :     void
 * Note :        You have to implement this function
 */
void ProcessScheduler::ExecuteFCFS()
{
	SortByArrivalTime();

	int time = 0;

	for (const auto process : mProcesses)
	{
		// If the process has not yet arrived, keep "waiting"
		while (time < process->GetArrivalTime())
		{
			time++;
		}

		// Once the process has arrived, it's time to start it
		process->SetStartTime(time);

		while (process->GetRemainingTime() > 0)
		{
			process->AddTimeSlot(time);
			process->DecreaseRemainingTime();

			time++;
		}
		
		process->SetFinishTime(time);
	}
}

/*
 * Name :        ProcessScheduler::ExecuteSPN()
 * Description : schedule the process based on "shortest process next"
 * Parameters :  void
 * Returns :     void
 * Note :        You have to implement this function
 */
void ProcessScheduler::ExecuteSPN()
{
	// First, we sort by arrival time
	SortByArrivalTime();

	int time = 0;

	// Keep track of the ready processes
	list<Process *> readyProcesses;

	// While there are still ready processes
	while (!AllProcessesDone())
	{
		// Check if other processes are available to be run.
		SetReadyProcesses(time, readyProcesses);

		// Now, other process that have arrived and are scheduled will be in readyProcesses
		// and currently sorted by arrival time.
		// Sort them by service time, as the processes with the shortest service time go next.
		readyProcesses.sort(CompareServiceTime);

		Process * process = readyProcesses.front();
		readyProcesses.pop_front();

		process->SetStartTime(time);
		while (process->GetRemainingTime() > 0)
		{
			process->AddTimeSlot(time);
			process->DecreaseRemainingTime();

			time++;
		}
		process->SetFinishTime(time);
	}
}

/*
 * Name :        ProcessScheduler::ExecuteSRT()
 * Description : schedule the process based on "shortest remaining time"
 * Parameters :  void
 * Returns :     void
 * Note :        You have to implement this function
 */
void ProcessScheduler::ExecuteSRT()
{
	SortByArrivalTime();

	int time = 0;

	list<Process *> readyProcesses;

	// While there are still processes to be ran
	while (!AllProcessesDone())
	{
		SetReadyProcesses(time, readyProcesses);
		readyProcesses.sort(CompareRemainingTime);

		Process * process = readyProcesses.front();
		readyProcesses.pop_front();

		// If the process has not started yet, set the start time
		if (!process->IsScheduled())
		{
			process->SetStartTime(time);
		}
		process->AddTimeSlot(time);
		process->DecreaseRemainingTime();
		
		time++;
		
		// If the process has finished running
		if (process->GetRemainingTime() <= 0)
		{
			// SetFinishTime will also mark the process as Done
			process->SetFinishTime(time);
		}
	}
}

/*
 * Name :        ProcessScheduler::ExecuteRR()
 * Description : schedule the process based on "round robin"
 * Parameters :  void
 * Returns :     void
 * Note :        You have to implement this function
 */
void ProcessScheduler::ExecuteRR()
{
	if(GetQuantum() <= 0)
	{
		cerr<<"For -RR option, you need to provide a quantum value > 0"<<endl;
		return;
	}

	SortByArrivalTime();

	int time = 0;

	// NOTE: A list is used here instead of a queue so that std::find() can find a specific process.
	// For all functional purposes, it works as a queue.
	list<Process *> readyProcesses;
	SetReadyProcesses(time, readyProcesses);

	while (!AllProcessesDone())
	{
		// Grab the first process from the list (queue),
		// and remove it from the queue.
		Process * process = readyProcesses.front();
		readyProcesses.pop_front();

		// If the process has not yet started, set start time
		if (!process->IsScheduled())
		{
			process->SetStartTime(time);
		}

		// Run the process for the time quantum
		for (int i = GetQuantum(); i > 0; i--)
		{
			process->AddTimeSlot(time);
			process->DecreaseRemainingTime();
			time++;

			// Arriving processes should be placed in the queue (if not already in the queue) before the current process re-enters
			AddArrivingProcessesToQueue(time, readyProcesses);

			// The process may be finished during the time quantum,
			// so break out of the loop if that's the case.
			if (process->GetRemainingTime() <= 0)
			{
				break;
			}
		}

		// If the process is done, set the finish time, which will also mark the process as done
		// so that it is not re-added to the queue.
		if (process->GetRemainingTime() <= 0)
		{
			process->SetFinishTime(time);
		}

		// Otherwise, re-add that process back to the queue.
		else
		{
			readyProcesses.push_back(process);
		}
	}
}

/*
 * Name :        ProcessScheduler::PrintToScreen()
 * Description : print the time slices together with the ID of the process scheduled for
 * 				 each time slice on the screen chronologically
 * Parameters :  void
 * Returns :     void
 * Note :        You have to implement this function
 */
void ProcessScheduler::PrintToScreen()
{
	vector<pair<int, int>> timeSlotProcessIDPairs;

	for (const auto process : mProcesses)
	{
		for (int i = 0; i < process->GetNumberOfTimeSlots(); i++)
		{
			timeSlotProcessIDPairs.push_back(pair<int, int>(process->GetTimeSlot(i), process->GetID()));
		}
	}

	// Sort the timeSlotProcessIDPairs by time (pair.first)
	sort(timeSlotProcessIDPairs.begin(), timeSlotProcessIDPairs.end(), TimeSlotSorter);

	for (const auto timeSlotPair : timeSlotProcessIDPairs)
	{
		cout << timeSlotPair.first << " " << timeSlotPair.second << endl;
	}
}

/*
 * Name :        ProcessScheduler::SortByServiceTime()
 * Description : sort the processes by ServiceTime (increasingly)
 * Parameters :  void
 * Returns :     void
 */
void ProcessScheduler::SortByServiceTime()
{
	sort(mProcesses.begin(), mProcesses.end(), CompareServiceTime);
}

/*
 * Name :        ProcessScheduler::SortByProcessID()
 * Description : sort the processes by ProcessID (increasingly)
 * Parameters :  void
 * Returns :     void
 */
void ProcessScheduler::SortByProcessID()
{
	sort(mProcesses.begin(), mProcesses.end(), CompareProcessID);
}

/*
 * Name :        ProcessScheduler::SortByRemainingTime()
 * Description : sort the processes by RemainingTime (increasingly)
 * Parameters :  void
 * Returns :     void
 */
void ProcessScheduler::SortByRemainingTime()
{
	sort(mProcesses.begin(), mProcesses.end(), CompareRemainingTime);
}

/*
 * Name :			ProcessScheduler::SortByArrivalTime()
 * Description : 	sort the processess by ArrivalTime (increasingly)
 * Parameters : 	void
 * Returns : 		void
 */
void ProcessScheduler::SortByArrivalTime()
{
	sort(mProcesses.begin(), mProcesses.end(), CompareArrivalTime);
}

/*
 * Name :			ProcessScheduler::SetReadyProcesses
 * Description :	Mark all processes that haven't finished to be ready if they have arrived.
 * 					Then, add those processes to a list.
 * Parameters :		int time, the current time of the CPU
 * 					list<Process *> the list of readyProcesses to add to
 * Returns :		void
 */
void ProcessScheduler::SetReadyProcesses(const int time, list<Process *> & readyProcesses)
{
	for (const auto process : mProcesses)
	{
		// If the process is not done
		// and the process is arriving or already arrived at the current time,
		// and the process hasn't already been added to the list
		if (!process->IsDone() && (time >= process->GetArrivalTime()) && find(readyProcesses.begin(), readyProcesses.end(), process) == readyProcesses.end())
		{
			process->SetIsReady();
			readyProcesses.push_back(process);
		}
	}
}

/*
 * Name :			ProcessScheduler::AddArrivingProcessesToQueue
 * Description :	Add processes that have arrived and have NOT started yet to the queue.
 * Parameters :		int time, the current time of the CPU
 * 					list<Process *> the list of readyProcesses to add to
 * Returns :		void
 */
void ProcessScheduler::AddArrivingProcessesToQueue(const int time, list<Process *> & readyProcesses)
{
	for (const auto process : mProcesses)
	{
		// Add the process to the queue if !IsScheduled() and it has arrived, and it's not already in the queue
		if (!process->IsScheduled() && (time >= process->GetArrivalTime()) && find(readyProcesses.begin(), readyProcesses.end(), process) == readyProcesses.end())
		{
			process->SetIsReady();
			readyProcesses.push_back(process);
		}
	}
}

// Returns true if all processes are finished
bool ProcessScheduler::AllProcessesDone()
{
	for (const auto process : mProcesses)
	{
		if (!process->IsDone())
		{
			return false;
		}
	}

	return true;
}




