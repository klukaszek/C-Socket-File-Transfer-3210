#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX_FILENAME_LEN 256

// Used to store timing information from the server
typedef struct
{
    time_t start_sec;
    long start_nsec;

    time_t end_sec;
    long end_nsec;
} TimingInfo;

// Function to send a file to the server
void sendFile(int clientSocket, const char *filename, int bufSize);
