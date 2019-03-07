// CS 270: Project 5
// File: smallSet.cpp
// Authors: Josh Luttrell, Devon Schleyer
// Description: Contains the declaration and execution
//		of the smallDigest program
//		
//
//------------------------------------------------------


#include "smLib.h"

using namespace std;

int main(int argc, char **argv)
{
	char *MachineName;
	int SecretKey;
	string data;
	int dataLength;
	string result;
	short resultLength;
	int port;
	char *host;

	if (argc != 5) {
		fprintf(stderr, "usage: %s <MachineName> <port> <SecretKey> <variableName>\n", argv[0]);
		exit(0);
	}
	MachineName = argv[1];
	port = atoi(argv[2]);
	SecretKey = atoi(argv[3]);
	data = argv[4];
	

	int retVal = smallDigest(MachineName,port,SecretKey,data,dataLength,result,resultLength);
	
	if(retVal == 0)
		return 0;

	return -1;
}
