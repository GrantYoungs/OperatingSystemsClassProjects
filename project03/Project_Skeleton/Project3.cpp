//============================================================================
// Name        : Project3.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : CSE410 Project3 Template
//============================================================================

#include <iostream>
#include "ProcessScheduler.h"
using namespace std;

/*
 * Name :        main()
 * Description : the main function of the program.
 * Parameters :  command line input for input file, output file, and option
 * Returns :     0 if the program runs successfully.
 *               1 if error occurs.
 */
int main(const int argc,const char *argv[])
{
	// check if the user invoke the program
	// together with command line arguments
	// e.g. "./myScheduler inputFile outputFile -option <argument>"
	if(argc > 3)
	{
		ProcessScheduler scheduler(argc, argv);
		if(!(scheduler.GetOption() == ProcessScheduler::RR && argc < 5))
		{
			// everything is fine, we start our process scheduling...
			scheduler.Execute();
			scheduler.PrintToScreen();
			cout<<"Done!"<<endl;
			return 0;
		}
	}

	// if the number of arguments is not correct, print out error message
	cout<<"Usage: "<<argv[0]<<" inputFile outputFile -option <arguments>"<<endl;
	return 1;
}
