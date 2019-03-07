// CS 270: Project 5
// File: smLib.h
// Authors: Josh Luttrell, Devon Schleyer
// Description: The functionality declaration of
//		smallSet, smallGet, and smallDigest
//
//------------------------------------------------------

extern "C"{
	#include "csapp.h"
}
#include <iostream>
#include <string>

using namespace std;

int smallSet(char *MachineName, int port, int SecretKey, string variableName, string value, short dataLength);

int smallGet(char *MachineName, int port, int SecretKey, string variableName, string value, short resultLength);

int smallDigest(char *MachineName, int port, int SecretKey, string data, int dataLength, string result, short resultLength);
