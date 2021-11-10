//============================================================================
// Name        : Project4.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : CSE410 Project4 Template
//============================================================================

#include <iostream>
#include "DotProduct.h"
using namespace std;

int main(int argc, const char* argv[])
{
	if(argc != 3)
	{
		cout<<"Usage: myProgram #ofValues option "<<endl;
		return 1;
	}

	/*
	 * Read in the file
	 * then, do the dot product operation
	 * print the result on the screen
	 */
	DotProduct dp;
	dp.Initialize(argc, argv);
	dp.Start();

	return 0;
}
