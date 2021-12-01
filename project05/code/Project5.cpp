//============================================================================
// Name        : Project5.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include "VirtualMemoryManager.h"
using namespace std;

// the function should accept the following options
const string OPTIONS = "a:f:p:P:r:t:";

int main(const int argc, char *argv[])
{
	// create a VirtualMemoryManager object
	VirtualMemoryManager myManager;

	int opt;

	// change the parameters according to the user input
	while((opt = getopt(argc, argv, OPTIONS.c_str())) != -1)
	{
		string argument;
		argument  = optarg;

		switch(opt)
		{
		// change algorithm
		case 'a':
			myManager.SetAlgorithm(argument);
			break;
		// change number of frames
		case 'f':
			myManager.SetMemoryFrameNumber(atoi(argument.c_str()));
			break;
		// change the page offset (0~23)
		case 'p':
			myManager.SetPageOffset(atoi(argument.c_str()));
			break;
		// change the number of process
		case 'P':
			myManager.SetProcessNumber(atoi(argument.c_str()));
			break;
		// change total reference number
		case 'r':
			myManager.SetTotalReferenceNumber(atoi(argument.c_str()));
			break;
		// change TLB size
		case 't':
			myManager.SetTLBSize(atoi(argument.c_str()));
			break;
		// the option can not be recognized
		case '?':
			cout<<"Cannot recognize!!"<<endl;
			break;
		}
	}

	// start our simulation
	myManager.Start();

	cout<<"done!"<<endl;

	// we are done
	return 0;
}
