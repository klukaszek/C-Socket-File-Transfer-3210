# CIS 3210 Networks - Assignment 1

Author: Kyle Lukaszek
ID: 1113798
Due: October 3rd, 2023

## Final Grade: 100%

### Description

There are two programs in this assignment. 

#### 1. sendFile
The first is a simple client that sends a file to a server and then reports the time it took for the file to be processed by the server, as well as the throughput. See usage below for more information.

#### 2. server
The second program is a simple server that receives a file from a client, saves it to "bin/received_files/", and then reports the time it took to receive and save the file back to the client. Included is a makefile that will compile both programs. The client and server can be run on the same machine or on different machines. The server must be started before the client. See usage below for more information.

### Test Scripts
There are 2 python scripts provided in the bin/ directory to test the client and server. 

To use these scripts you must first make sure that the server is running and that you have Python installed. 

More information about the scripts exists within the scripts themselves.

#### 1. spawn_clients.py
The first script, spawn_clients.py, is designed to test multiple clients sending files to the server at the same time. The server is programmed to have a listen queue of 20 connections so you should only use this script to spawn 20 clients at a time. Last I checked, it was able to handle all 20 clients at once. The script will print everything in the incorrect order, you can just ignore this and look at the server instead to see that the files were received (in this case we are sending the same file 20 times so it should just be 1 file). See usage below for more information.

#### 2. environment_test.py
The second test script, environment_test.py, sequentially tests the client and server while keeping track of transfer times, and throughput. Once all clients have finished sending files, the script will print the min, max, and average transfer time and throughput for the clients. See usage below for more information.

### Project Structure

- bin/ - contains the compiled programs and test scripts
    - received_files/ - contains the files that the server receives from the client
- include/ - contains the .h header files for the programs
- src/ - contains the source code .c files for the programs
- Makefile - compiles the programs
- Assignment 1 Report.pdf - contains the report for this assignment
- README.md - this file

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
