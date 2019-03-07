// CS 270: Project 5
// File: smLib.cpp
// Authors: Josh Luttrell, Devon Schleyer
// Description: The functionality and implementation of
//		smallSet, smallGet, and smallDigest
//
//------------------------------------------------------

extern "C"
{
	#include "csapp.h"
}

#include "smLib.h"
#include <iostream>
#include <string>

#define INT_SIZE 4
#define PAD_SIZE 2
#define LARGE_PAD_SIZE 3
#define TYPE_SIZE 2
#define SHORT_DATA_LENGTH 2
#define MAX_VAR_NAME 15
#define MAX_VAR_VAL 100
using namespace std;


int smallSet(char *MachineName, int port, int SecretKey, string variableName, string value, short dataLength)
{
	int clientfd;
	rio_t rio;

	short type = 0;
	short pad = 350;		// Arbitrary value

	clientfd = Open_clientfd(MachineName, port);
	Rio_readinitb(&rio, clientfd);	
	
	// Convert key to byte order
	SecretKey = htonl(SecretKey);	
	// Sends key to server
	Rio_writen(clientfd, &SecretKey, INT_SIZE);

	// Converts type byte order
	type = htons(type);
	// Sends type to server
	Rio_writen(clientfd, &type, TYPE_SIZE);
	
	// Converts pad byte order
	pad = htons(pad);
	// Sends pad to server
	Rio_writen(clientfd, &pad, PAD_SIZE);						

	// Sends varName to server 
	Rio_writen(clientfd, &variableName[0], MAX_VAR_NAME);	
	
	// Converts dataLength to byte order
	dataLength = htons(dataLength);
	// Sends dataLength to server
	Rio_writen(clientfd, &dataLength, SHORT_DATA_LENGTH);		
	

	// Sends value to server 
	Rio_writen(clientfd, &value[0], MAX_VAR_VAL);


	close(clientfd);
	return 0;
}

int smallGet(char *MachineName, int port, int SecretKey, string variableName, string value, short resultLength)
{
	int clientfd;
	rio_t rio;
	int connfd;
	
	short length;
	
	short type = 1;
	short smallPad = 350;
	char pad[3] = {'1','2','3'};		// Arbitrary Value
	char status;
	
	clientfd = Open_clientfd(MachineName, port);
	Rio_readinitb(&rio, clientfd);

	// Convert key to byte order
	SecretKey = htonl(SecretKey);	
	// Sends key to server
	Rio_writen(clientfd, &SecretKey, INT_SIZE);

	// Converts type byte order
	type = htons(type);
	// Sends type to server
	Rio_writen(clientfd, &type, TYPE_SIZE);
	
	// Converts pad byte order
	smallPad= htons(smallPad);
	// Sends pad to server
	Rio_writen(clientfd, &smallPad, PAD_SIZE);						
	// Sends varName to server 
	Rio_writen(clientfd, &variableName[0], MAX_VAR_NAME);	

	// Reads the status from the server
	Rio_readn(clientfd, &status, 1);

	// Prints the value when the status succeeds
	if((int)status == 0)
	{	
		// Reads pad from server
		Rio_readn(clientfd, &pad, LARGE_PAD_SIZE);
	

		// Reads length from server
		Rio_readn(clientfd, &length, SHORT_DATA_LENGTH);
		length = ntohs(length);
					

		// Reads value from server
		string readValue;
		Rio_readn(clientfd, &readValue[0], 100);
		value = &readValue[0];
		

		cout << value.c_str() << endl;
	}
	
	// Prints error when the status fails
	else
	{
		Rio_readn(clientfd, &pad, LARGE_PAD_SIZE);
	
		cout << "failed" << endl;

		return -1;
	}

	
	Close(clientfd); //line:netp:echoclient:close
	return 0;
}

int smallDigest(char* MachineName, int port, int SecretKey, string data, int dataLength, string result, short resultLength)
{
	int clientfd;
	rio_t rio;

	// smallDigest client info
	short type = 2;
	short smallPad = 350;
	char pad[3] = {'1','2','3'};		// Arbitrary Value


	clientfd = Open_clientfd(MachineName, port);
	Rio_readinitb(&rio, clientfd);

	// Convert key to byte order
	SecretKey = htonl(SecretKey);	
	// Sends key to server
	Rio_writen(clientfd, &SecretKey, INT_SIZE);

	// Converts type byte order
	type = htons(type);
	// Sends type to server
	Rio_writen(clientfd, &type, TYPE_SIZE);
	
	// Converts pad byte order
	smallPad = htons(smallPad);
	// Sends pad to server
	Rio_writen(clientfd, &smallPad, PAD_SIZE);			
	
	// Sends length to server
	short length = data.size();
	length = htons(length);
	Rio_writen(clientfd, &length, SHORT_DATA_LENGTH); 		
		
	// Sends data to server 
	Rio_writen(clientfd, &data[0], MAX_VAR_VAL);	

	// Reads status from server
	char status;
	Rio_readn(clientfd, &status, 1);

	// Reads pad from server
	Rio_readn(clientfd, &pad, LARGE_PAD_SIZE);

	// Reads resultLength from server
	Rio_readn(clientfd, &resultLength, SHORT_DATA_LENGTH);
	resultLength = ntohs(resultLength);
	
	// Reads result from server
	Rio_readn(clientfd, &result[0], MAX_VAR_VAL);

	cout << result.c_str();

	Close(clientfd); //line:netp:echoclient:close
	exit(0);
}
