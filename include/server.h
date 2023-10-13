// CIS 3210 - Assignment 1
// Author: Kyle Lukaszek
// ID: 1113798
// Due: October 13, 2023

#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
 
#define MAXRCVLEN 500
#define MAX_FILENAME_LEN 256

// Function that will be called when SIG_INT is caught so we can close the sockets and exit gracefully
static void sigintCatcher(int signal,  siginfo_t* si, void *arg);

// Function that handles receiving a file in chunks from the client
int receiveFile(int socket, int size);