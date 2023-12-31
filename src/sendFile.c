// CIS 3210 - Assignment 1
// Author: Kyle Lukaszek
// ID: 1113798
// Due: October 13, 2023

#include "../include/sendFile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

int main(int argc, char *argv[]) 
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <fileName> <server-IP-address:port-number> <optional:bufSize>\n", argv[0]);
        exit(1);
    }

    const char *filename = argv[1];
    const char *serverAddressAndPort = argv[2];
    int buf_size = 4096; // default buffer size per assignment instructions

    // Check if buffer size was specified
    if (argc == 4) 
    {
        buf_size = atoi(argv[3]);
        if (buf_size < 1) {
            fprintf(stderr, "Buffer size must be greater than 0\n");
            exit(1);
        }
    } 

    // Extract server address and port from the combined argument
    char server_address[100]; // Adjust the size as needed
    int port_number;
    if (sscanf(serverAddressAndPort, "%99[^:]:%d", server_address, &port_number) != 2) {
        fprintf(stderr, "Invalid server address format. Use 'server-IP-address:port-number'.\n");
        exit(1);
    }

    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Resolve server address
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port_number);

    struct hostent *host = gethostbyname(server_address);
    if (host == NULL) {
        perror("Unable to resolve server address");
        close(client_socket);
        exit(1);
    }
    memcpy(&serverAddr.sin_addr, host->h_addr_list[0], host->h_length);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(1);
    }

    // Send the filename and file data to the server
    sendFile(client_socket, filename, buf_size);

    // Receive the total time (in seconds) from the server
    double total_time_sec = 0;
    if (recv(client_socket, &total_time_sec, sizeof(double), 0) == -1) {
        perror("Failed to receive timing info from server");
        return -1;
    }

    // Close the socket
    close(client_socket);

    // Calculate the total time in milliseconds
    double total_time_ms = total_time_sec * 1000;

    // Get the file size
    FILE *file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fclose(file);

    // Calculate the throughput in kilobytes/second
    double throughput_KBps = ((double)file_size / 1024.0) / total_time_sec;

    // Print the total time (milliseconds) and throughput (kilobytes/second) to stdout
    printf("Total time: %.5f ms\n", total_time_ms);
    printf("Throughput: %.5f KB/s\n", throughput_KBps);

    return 0;
}

// Function to send a file to the server
void sendFile(int client_socket, const char *filename, int buf_size) 
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Get filename len
    int filename_len = strlen(filename);

    // Calculate the number of chunks (handle the case where there is a remainder)
    // Use long in the event that the file is large and a small buffer size is used (i.e. 1)
    long num_chunks = file_size / buf_size + (file_size % buf_size == 0 ? 0 : 1);

    printf("Sending file %s (%ld bytes) in %ld chunks\n", filename, file_size, num_chunks);

    // Send the filename len, filename, file size, and number of chunks to the server
    send(client_socket, &filename_len, sizeof(int), 0);
    send(client_socket, filename, strlen(filename), 0);
    send(client_socket, &file_size, sizeof(long), 0);
    send(client_socket, &num_chunks, sizeof(long), 0);

    // Send the file data in chunks
    char buffer[buf_size];
    while (!feof(file)) {
        int bytesRead = fread(buffer, 1, buf_size, file);
        if (bytesRead == 0) break;
        send(client_socket, buffer, bytesRead, 0);
    }

    fclose(file);
}