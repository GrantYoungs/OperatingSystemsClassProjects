/*
 * File: TLB.cpp
 *
 * The class for handling the TLB
 *
 * Header: TLB.h
 * Author: Clifton Howard
 *notes: TLB is stored as a queue front = oldest , back = newest
 */

#include "VirtualMemoryManager.h"
#include "ReferenceGenerator.h"
#include "Reference.h"
#include "TLB.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <math.h>
using namespace std;

/*
 * Name : TLB::TLB(int s ,VirtualMemoryManager* m)
 * Description :constructor for TLB object
 * Note :
 */
TLB::TLB(int s ,VirtualMemoryManager* m)
{
  size = s;
 // cout << "new tlb created\n";
  returnframeIndex = -1 ;//m->GetMemoryFrameNumber() + 1;

}
/*
 * Name :TLB::~TLB()
 * Description :destructor for TLB object
 * Note :
 */
TLB::~TLB()
{

}
/*
 * Name : void TLB::init()
 * Description :initialize tlb properties
 * Note :  not used yet
 */
void TLB::init()
{


}
/*
 * Name : int TLB::find(int checkPID ,int page, int addr, int a, int m)
 * Description : check if a PID and Page is in the TLB
 * Note : if it finds in tlb place on the end of the QUEUE
          this way the front of the queue is odlest accessed and
          back is newest accessed
 */
int TLB::find(int checkPID ,int page, int addr, int a, int m)
{
  int check = page;
  returnframeIndex = -1;
  bool found = false;
  // You should Implement it! Insert a TLBRecord to the TLB

  return returnframeIndex;

}
/*
 * Name : bool TLB::insert(int checkPID ,int page, int addr, int a, int m)
 * Description :insert an entry into the TLB
 * Note :
 */
bool TLB::insert(int checkPID ,int page, int addr, int a, int m)
{
  int check = page;
  TLBRecord temp;
  temp.pageNum = page;
  temp.frameIndex = addr;
  temp.used = a;
  temp.modified = m;
  // You should Implement it! Insert a TLBRecord to the TLB
}

/*
 * Name : void TLB::clear()
 * Description :clears out TLB queue by poping all elements
 * Note :
 */
void TLB::clear()
{
  for(int i = 0; i < theTLB.size(); i++)
  {
    theTLB.pop();
  }

}

/*
 * Name : void TLB::remove(int oldPID, int oldPage)
 * Description :removes an entry from TLB
 * Note : Finds entry using pagenum and PID
 */
void TLB::remove(int oldPID, int oldPage)
{
  TLBRecord temp;

  int TLBsize = theTLB.size();
  for(int i = 0; i < TLBsize; i++)
  {
    temp = theTLB.front();
    theTLB.pop();
    if(temp.pageNum != oldPage && oldPID != PID)
    {
      theTLB.push(temp);
    }
  }

}

/*
 * Name : void TLB::print()
 * Description :prints out TLB contenets
 * Note :  used for debugging purposes
 */
void TLB::print()
{
  cout << "TLB [ ";
  for(int i = 0; i < theTLB.size(); i++)
  {
    TLBRecord temp = theTLB.front();
    cout << temp.pageNum << ", " ;

    theTLB.pop();
    theTLB.push(temp);
  }
  cout << "]\n";
}
