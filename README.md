# CIS 3210 Networks - Assignment 1

## Author: Kyle Lukaszek

### Description

There are two programs in this assignment. The first is a simple client that sends a file to a server and then reports the time it took for the file to be processed by the server, as well as the throughput. The second program is a simple server that receives a file from a client, saves it, and then reports the time it took to receive and save the file back to the client. Included is a makefile that will compile both programs. The client and server can be run on the same machine or on different machines. The server must be started before the client.

There are 2 python scripts provided in the bin/ directory to test the client and server. To use these scripts you must first make sure that the server is running and that you have Python installed. The first script, spawn_clients.py, is designed to test multiple clients sending files to the server at the same time. This script will only work if you follow the instructions provided at the top of the file since it requires a specific directory structure. This is the case because if we spawn multiple clients trying to read and then write to the same file at once 

### Usage

#### Client

``` bash
./sendFile <filename> <serverIP:port> <optional: bufferSize>
```

#### Server

``` bash
./server <port> <optional: bufferSize>
```