#!/usr/bin/env python3

# CIS 3210 - Assignment 1
# Author: Kyle Lukaszek
# ID: 1113798
# Due: 10-13-2023

# This script spawns a number of clients to send a file to a server.
# Usage: python spawn_clients.py filename server-IP:port num_clients optional:buf_size

""" 
IMPORTANT NOTE:
This script doesn't actually work without editing server.c to write the received file to a different directory.
I have tested this script with the following changes to server.c and it works as expected.

First create a directory to store the received files in bin/:

In server.c at line 187:

char dir = "{some directory}/";
    
Then in server.c at line 188:

strcat(dir, filename);

Then below that replace the following:

// Open the file for writing
file = fopen(filename, "wb");
if (file == NULL) {
    perror("\tFailed to create file");
    return -1;
}

with this:

// Open the file for writing
file = fopen(dir, "wb");
if (file == NULL) {
    perror("\tFailed to create file");
    return -1;
}
"""

# Note 2: The script will exit before the clients are finished, but the clients will continue to run until they finished.
# Note 3: The output will be mixed together, just ignore it since it's not important in this case.

import sys
import subprocess
import multiprocessing

def spawn_clients(filename, server_address, num_clients, buf_size=-1):

    """ 
    Spawns a number of clients to send a file to a server. 
    """
    def run_client():
        command = []
        if buf_size == -1:
            command = ["./sendFile", filename, server_address]
        else:
            command = ["./sendFile", filename, server_address, str(buf_size)]
        subprocess.Popen(command)

    processes = []
    for _ in range(num_clients):
        process = multiprocessing.Process(target=run_client)
        process.start()
        processes.append(process)

    for process in processes:
        process.join()

if len(sys.argv) < 4:
    print("Usage: python spawn_clients.py filename server-IP:port num_clients optional:bufsize")
    sys.exit(1)

def main():

    """ Main function """

    filename = sys.argv[1]
    server_address = sys.argv[2]
    num_clients = int(sys.argv[3])

    if len(sys.argv) == 5:
        bufsize = int(sys.argv[4])
        spawn_clients(filename, server_address, bufsize, num_clients)
    else:
        spawn_clients(filename, server_address, num_clients)

    print(f"Spawning {num_clients} clients to send {filename} to {server_address}")

    sys.exit(0)

if __name__ == "__main__":
    main()
