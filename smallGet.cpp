// CS 270: Project 5
// File: smallSet.cpp
// Authors: Josh Luttrell, Devon Schleyer
// Description: Contains the declaration and execution
//		of the smallGet program
//		
//
//------------------------------------------------------


#include "smLib.h"

using namespace std;

int main(int argc, char **argv)
{
	int port;
	char *MachineName;
	int SecretKey;
	string variableName;
	string value;
	short resultLength;

	if (argc != 5) {
		fprintf(stderr, "usage: %s <MachineName> <port> <SecretKey> <variableName>\n", argv[0]);
		exit(0);
	}
	MachineName = argv[1];
	port = atoi(argv[2]);
	SecretKey = atoi(argv[3]);
	variableName = argv[4]; 

	int retVal = smallGet(MachineName,port,SecretKey,variableName,value,resultLength);
	
	if(retVal == 0)
		return 0;

	return -1;
}
