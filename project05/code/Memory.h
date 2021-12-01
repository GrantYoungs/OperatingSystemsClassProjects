/*
 * File    : Memory.h
 *
 * Header for the Memory class for Project5.
 *
 * Class   : Memory.h
 * Impl    : Memory.cpp
 * Created : april 18, 2012
 * Author  : Clifton Howard
 *
 * Notes   : class to handle the memory frame storage and operations
 */

#ifndef Memory_H_
#define Memory_H_

#include "TLB.h"
#include <queue>
#include <string>

class Reference;
class TLB;
class VirtualMemoryManager;

class Memory
{
public:
	//enum Operation {Read = 0, Write = 1};
	Memory(VirtualMemoryManager* m);
	virtual ~Memory();

	void init();
	// Setters
	void SetAlgorithm(std::string &s){Algorithm = s;}

	void setNextTime(int index , int time){theMem[index].nextTime = time;}
	// access memory loacation

	// insert page info into a frame
	int insert(int PID ,int page, int a, int m, int t, int &index, int & , int &);



	void print();


	// record to store info of what the frame contains
	struct FrameRecord
	{
	  int refPID;
	  int pageNum;
	  //int frameIndex;
	  int used;
	  int modified;
	  bool empty ;
	  int time;
	  int nextTime;
	};

private:


	int PID; // PID of process using the TLB
	int size; // max size of TLB

	FrameRecord * theMem;
	int pointer; // point to next spot to insert.
	std::string Algorithm ;
	VirtualMemoryManager* myManager;
	int faults; // stores number of page faults
	int clkptr;


};

#endif /* Memory_H_ */
