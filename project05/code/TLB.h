/*
 * File    : TLB.h
 *
 * Header for the TLB class for Project5.
 *
 * Class   : TLB.h
 * Impl    : TLB.cpp
 * Created : april 18, 2012
 * Author  : Clifton Howard
 *
 * Notes   : header for class that creates a TLB and performs operations on it
 */

#ifndef TLB_H_
#define TLB_H_

#include <queue>

class Reference;
class VirtualMemoryManager;

class TLB
{
public:
	//enum Operation {Read = 0, Write = 1};
	TLB(int s ,VirtualMemoryManager* m);
	virtual ~TLB();

	// Setters

	void SetUserID(const int i){PID = i;}



	// Getters
	int GetUserID() const {return PID;}


	int find(int thePID, int page, int addr, int a, int m);
	bool insert(int thePID, int page, int addr, int a, int m);
	void clear();
	void remove(int , int );
	void setSize(int s){size = s;}
	void init();
	void print();
	int getPID(){return PID;}
	int setPID(int newPID){PID = newPID;}


	struct TLBRecord
	{
	  int PID;
	  int pageNum;
	  int frameIndex;
	  int used;
	  int modified;
	};

private:


	int PID; // PID of process using the TLB
	int size; // max size of TLB
	int returnframeIndex;
	std::queue<TLBRecord> theTLB ;


};

#endif /* TLB_H_ */
