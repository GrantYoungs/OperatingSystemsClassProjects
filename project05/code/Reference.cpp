/*
 * File: Reference.cpp
 *
 * The class of reference
 *
 * Header: Reference.h
 * Author: CSE410 Spring 2012
 *
 */

#include "Reference.h"
#include <iostream>
using namespace std;

/*
 * Name :        Reference::Reference()
 * Description : constructor
 * Note :
 */
Reference::Reference()
{
	mProcessID = 0;
	mVirtualAddress = 0;
	mOperation = Read;
}

/*
 * Name :        Reference::~Reference()
 * Description : Default destructor
 * Note :
 */
Reference::~Reference()
{
	// TODO Auto-generated destructor stub
}

/*
 * Name :        Reference::SetVirtualAddress()
 * Description : set the virtual address by shifting the
 * 				 page number left "pageOffset" bits.
 * Note :
 */
void Reference::SetVirtualAddress(const int i, const int pageOffset)
{
	// shift left by "pageOffset" bits
	mVirtualAddress = (i << pageOffset);
}

/*
 * Name :        Reference::Print()
 * Description : print the content of the reference
 * Note :
 */
void Reference::Print()
{
	cout<<"Reference: PID="<< this->GetProcessID()<<" Add="<<this->GetVirtualAddress()<<" Opt="<<this->GetOperation()<<endl;
}








