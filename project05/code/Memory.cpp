/*
 * File: Memory.cpp
 *
 * The class for handling the memory frames
 *
 * Header: VirtualMemoryManager.h
 * Author: Clifton Howard
 *
 */

#include "VirtualMemoryManager.h"
#include "ReferenceGenerator.h"
#include "Reference.h"
#include "TLB.h"
#include "Memory.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <math.h>
using namespace std;



/*
 * Name :  Memory::Memory(VirtualMemoryManager* m)
 * Description : constructor for  Frame memory object
 * Note :
 */
Memory::Memory(VirtualMemoryManager* m)
{

  //cout << "new memory Frame created\n";
  size = m->GetMemoryFrameNumber();
  theMem = new FrameRecord[size];


  for( int i = 0; i < size; i++)
  {
    theMem[i].empty = true;
    theMem[i].refPID =  m-> GetProcessNumber() ;
    theMem[i].nextTime =  -1 ;
  }
  pointer = 0;

  faults = 0;

  myManager = m;
}


/*
 * Name :   Memory::~Memory()
 * Description : destructor for  Frame memory object
 * Note :
 */
Memory::~Memory()
{

}
/*
 * Name :   void Memory::init()
 * Description : initializer function
 * Note : unused
 */
void Memory::init()
{


}


/*
 * Name : Memory::insert(int PID, int page, int a, int m, int t, int &index, int& oldPID , int& oldPage)
 * Description :insert a page at a memory location
 * Note : uses FIFO , LRU , CLOCK , or OPT to find spot and insert a  page
 */
int Memory::insert(int PID, int page, int a, int m, int t, int &index, int& oldPID , int& oldPage)
{

  FrameRecord temp;
  temp.refPID = PID;
  temp.pageNum = page;

  temp.used = a;
  temp.modified = m;

  // if spot is empty
  if(theMem[pointer].empty)
  {
    // You need initialize the Memory, to insert a frame into an empty spot

  }
  else // cell is not empty so insert using correct algorithm
  {
    // Implement LRU, FIFO, CLOCK Function Here!

  }

}




/*
 * Name : void Memory::print()
 * Description :print of contenets of memory
 * Note : unused memory prints process id of numPRocess + 1;
 */
void Memory::print()
{

  cout <<  "pID        page#       Used        Modified\n";
  cout <<  "-------------------------------------------\n";

  for(int i = 0; i < size; i++)
  {
    cout.setf ( ios::left );
    cout << setw (11) << theMem[i].refPID ;
    cout << setw (12) << theMem[i].pageNum ;
    cout << setw (12) << theMem[i].used ;
    cout << setw (10) << theMem[i].modified << endl ;//<< endl;
    //cout << setw (10) << theMem[i].time ;

    //cout << setw (10) << theMem[i].nextTime << endl;
  }
  cout << " page fault # = " << faults << endl;

}
