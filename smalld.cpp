// CS 270: Project 5
// File: smalld.cpp
// Authors: Josh Luttrell, Devon Schleyer
// Description: Sets up a server whenever the program
//		smalld is running. This program communicates
//		back and forth with the client.
//
//
//-----------------------------------------------------------


extern "C"{
#include "csapp.h"
}

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

#define INT_SIZE 4
#define PAD_SIZE 2
#define LARGE_PAD_SIZE 3
#define SHORT_SIZE 2
#define SHORT_DATA_LENGTH 2
#define MAX_VAR_NAME 15
#define MAX_VAR_VAL 100

using namespace std;


struct clientInfo
{
	unsigned int clientKey;
	short requestType;
	string varName;
	string value;
	short padding;
	short  dataLength;

} ci;


int main(int argc, char **argv) 
{
	int listenfd, connfd, port;
	socklen_t clientlen;
	struct sockaddr_in clientaddr;
	struct hostent *hp;
	char *haddrp;	
	unsigned int SecretKey;
	if (argc != 3) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}

	// Used to store variable names and their value
	vector<string> varInfo;

	port = atoi(argv[1]);
	listenfd = Open_listenfd(port);
	while (1) 
	{
		clientlen = sizeof(clientaddr);
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

		// Determine the domain name and IP address of the client 
		hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
				sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		haddrp = inet_ntoa(clientaddr.sin_addr);
		//printf("server connected to %s (%s)\n", hp->h_name, haddrp);

		// The original key from the command line
		SecretKey = atoi(argv[2]);

		// Reads the client key from the server
		Rio_readn(connfd, &ci.clientKey, INT_SIZE);
		// Converts clientKey byte order
		ci.clientKey = ntohl(ci.clientKey);


		// Prints only the key whenver the clientKey is incorrect
		if(ci.clientKey != SecretKey)
		{
			cout << "Secret Key: " << ci.clientKey << endl;
			cout << "-------------------------------------------" << endl;

			Close(connfd);
			continue;
		}	
		// Prints the SecretKey
		cout << "Secret Key = " << ci.clientKey << endl;

		// Reads the type of request from the server 
		Rio_readn(connfd, &ci.requestType, SHORT_SIZE);
		// Converts request to original byte order
		ci.requestType = ntohs(ci.requestType);

		// Reads the padding from server
		Rio_readn(connfd, &ci.padding, PAD_SIZE);
		// Converts padding to original byte order		
		ci.padding = ntohs(ci.padding);


		if(ci.requestType == 0)
		{
			// Prints request type
			cout << "Request Type = set" << endl;

			// Reads varName from the server
			Rio_readn(connfd, &ci.varName[0], MAX_VAR_NAME);

			// Prints the varName
			cout << "Detail = " << ci.varName.c_str() << endl;

			// Reads dataLength from server
			Rio_readn(connfd, &ci.dataLength, SHORT_DATA_LENGTH);
			// Converts dataLength byte order
			ci.dataLength = ntohs(ci.dataLength);

			// Reads value from server
			Rio_readn(connfd, &ci.value[0], MAX_VAR_VAL);
	
			// Sets the current value equal to a variable name that already exists
			int present = -1;
			for(int i = 0; i < varInfo.size(); i+=2)
			{
				if(ci.varName.c_str() == varInfo[i])
				{
					present = i;
					varInfo[present+1] = ci.value.c_str();
				}
			}

			if( present == -1)
			{
				// Variable name is stored into even index elements
				// in the vector
				varInfo.push_back(ci.varName.c_str());
				varInfo.push_back(ci.value.c_str());
			}


			// Prints success
			cout << "Completion = Success" << endl;
			cout << "--------------------------------------" << endl;

		}

		else if(ci.requestType == 1)
		{
			// Prints request type
			cout << "Request Type = get" << endl;

			// Reads varName from the server
			Rio_readn(connfd, &ci.varName[0], MAX_VAR_NAME);

			cout << "Detail = " << ci.varName.c_str() << endl;


			// smallGet client info
			short resultLength;
			char pad[3] = {'1','2','3'};		
			char status = 1;
			int count = 0;
			for(int i = 0; i < varInfo.size(); i+=2)
			{
				// Checks to see if the variable has already been created
				if(varInfo[i] == ci.varName.c_str())
				{

					// Sets status to success and writes it to the client
					status = 0;
					Rio_writen(connfd, &status, 1);

					// Sends pad to the client
					Rio_writen(connfd, &pad, LARGE_PAD_SIZE);


					// Sets resultLength equal to the size of value
					resultLength = varInfo[i+1].size();

					// Converts byte order of resultLength and then
					// sends it to the client
					short copy = resultLength;
					resultLength = htons(resultLength);
					Rio_writen(connfd, &resultLength, SHORT_SIZE);


					// Sends the value to the client
					Rio_writen(connfd, &varInfo[i + 1][0], MAX_VAR_VAL);

					cout << "Completion = success" << endl;
				}

				// If not found then gives a failure status
				else
				{
					count+=2;
					if(count == varInfo.size())
					{
						// Gives a failure status and sends it to the client
						status = -1;
						Rio_writen(connfd, &status, 1);

						// Sends pad to the client
						Rio_writen(connfd, &pad, LARGE_PAD_SIZE); 

						cout << "Completion = failure" << endl; 
					}	

				}

			}

			if(varInfo.size() == 0)
			{
				// Gives a failure status and sends it to the client
				status = -1;
				Rio_writen(connfd, &status, 1);

				// Sends pad to the client
				Rio_writen(connfd, &pad, LARGE_PAD_SIZE); 

				cout << "Completion = failure" << endl; 

			}

			cout << "-----------------------------" << endl;

		}
		else if(ci.requestType == 2)
		{
			// Prints request type
			cout << "Request Type = digest" << endl;

			Rio_readn(connfd, &ci.dataLength, SHORT_SIZE);
			ci.dataLength = ntohs(ci.dataLength);

			// Reads varName from the server
			Rio_readn(connfd, &ci.value[0], MAX_VAR_VAL);

			// Prints the varName
			cout << "Detail = " << ci.value.c_str() << endl;

			cout << "Completion = success" << endl;
			cout << "-------------------------------" << endl;

			char status = 0;
			Rio_writen(connfd, &status, 1);

			char pad[3] = {'1','2','3'};		
			// Sends pad to the client
			Rio_writen(connfd, &pad, LARGE_PAD_SIZE); 

			
			// Creates a string that contains the echo invocation of the
			// variable value
			string invocation = "/bin/echo ";
			invocation += &ci.value[0];
			invocation += " | /usr/bin/sha256sum ";

			// Opens a file to store the output of the invocation
			FILE* digest = popen(invocation.c_str(), "r");
			string toWrite;

			// Prints to stderr if the file did not open
			if (!digest)
			{
				cerr << "Couldn't open file." << endl;	
			}

			// Reads the file content and sets it equal to a string
			else
			{
				while(fgets(const_cast<char *>(ci.value.c_str()), sizeof(const_cast<char *>(ci.value.c_str())), digest) != NULL)
				{
					toWrite += const_cast<char *>(ci.value.c_str());
				}
					
					// Sends the size of the toWrite string to the client
					short toWriteLength = toWrite.size();
					toWriteLength = htons(toWriteLength);
					Rio_writen(connfd, &toWriteLength, SHORT_SIZE);
						
					// Sends the toWrite string to the client
					Rio_writen(connfd, &toWrite[0], MAX_VAR_VAL);

			}
			
			// Closes the file
			pclose(digest);

		}



	}
	Close(connfd);
	exit(0);


}
