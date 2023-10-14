# CIS 3210 Networks - Assignment 1

## Author: Kyle Lukaszek

### Description

There are two programs in this assignment. The first is a simple client that sends a file to a server and then reports the time it took for the file to be processed by the server, as well as the throughput. The second program is a simple server that receives a file from a client, saves it to "bin/received_files", and then reports the time it took to receive and save the file back to the client. Included is a makefile that will compile both programs. The client and server can be run on the same machine or on different machines. The server must be started before the client.

There are 2 python scripts provided in the bin/ directory to test the client and server. To use these scripts you must first make sure that the server is running and that you have Python installed. The first script, spawn_clients.py, is designed to test multiple clients sending files to the server at the same time. The server is programmed to have a listen queue of 20 connections so you should only use this script to spawn 20 clients at a time. 

The second test script, environment_test.py, sequentially tests the client and server while keeping track of transfer times, and throughput. Once all clients have finished sending files, the script will print the min, max, and average transfer time and throughput for the clients.

### Makefile

``` bash
make all

make server

make client

make clean
```

### Usage

Server IPs can be replaced with urls as well.

#### Server

``` bash
./server <port> <optional: bufferSize>
```

#### Client

``` bash
./sendFile <filename> <serverIP:port> <optional: bufferSize>
```

#### Test script 1: spawn_clients.py

``` bash
python3 spawn_clients.py <filename> <serverIP:port> <number of clients> <optional: bufferSize>
```

#### Test script 2: environment_test.py

``` bash
python3 environment_test.py <filename> <serverIP:port> <number of clients> <optional: bufferSize>
```