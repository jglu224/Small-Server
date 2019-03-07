// CS 270: Project 5
// File: smallSet.cpp
// Authors: Josh Luttrell, Devon Schleyer
// Description: Contains the declaration and execution
//		of the smallSet program
//		
//
//------------------------------------------------------


#include "smLib.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
	int  port;	
	char *MachineName;
	int SecretKey;
	string variableName;
	string value;
	int dataLength;

	if (argc != 6) {
		fprintf(stderr, "usage: %s <MachineName> <port> <SecretKey> <variableName> <value>\n", argv[0]);
		exit(0);
	}

	MachineName = argv[1];
	port = atoi(argv[2]);
	SecretKey = atoi(argv[3]);
	variableName = argv[4];
	value = argv[5];
	dataLength = value.size();

	int retVal = smallSet(MachineName,port,SecretKey,variableName,value,dataLength);

	if(retVal == 0)
		return 0;

	return -1;
}
