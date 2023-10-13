#!/usr/bin/env python3

# CIS 3210 - Assignment 1
# Author: Kyle Lukaszek
# ID: 1113798
# Due: 10-13-2023

import re
import sys
import subprocess

# This script spawns a number of clients to send a file to a server and then reports the min, max, and average times and throughputs.
# Note: The clients are being run sequentially for the test. 
# Note 2: If you want to test the server handling multiple clients at once, then use the spawn_clients.py script.

def spawn_client(filename, server_address, bufsize=-1, output_data=None):

    """
    Runs the sendFile program with the given arguments and puts the output in the output_queue. 
    """

    if output_data is None:
        output_data = {'Times': [], 'Throughputs': []}

    command = []

    # If bufsize is -1, then the default buffer size will be used in sendFile
    if bufsize == -1:
        command = ["./sendFile", filename, server_address]
    else:
        command = ["./sendFile", filename, server_address, str(bufsize)]

    result = subprocess.run(command, capture_output=True, text=True, check=True)

    # Extracting relevant information using regular expressions
    output_lines = result.stdout.splitlines()

    # Iterate through the output from the subprocess and print/extract the relevant information
    for line in output_lines:
        if "Sending" in line:
            print(line)
        elif "Total time" in line:
            total_time = float(re.findall(r'\d+', line)[0])
            output_data['Times'].append(total_time)
        elif "Throughput" in line:
            throughput = float(re.findall(r'\d+', line)[0])
            output_data['Throughputs'].append(throughput)


def main():

    """
    Run multiple clients and then report the min, max, and average times and throughputs.
    """

    if len(sys.argv) < 4:
        print("Usage: python spawn_clients.py filename server-IP:port num_clients optional:bufsize")
        sys.exit(1)

    filename = sys.argv[1]
    server_address = sys.argv[2]
    num_clients = int(sys.argv[3])

    print(f"Spawning {num_clients} clients to send {filename} to {server_address}")

    results = {'Times': [], 'Throughputs': []}

    # If the user specified a buffer size, then use it
    if len(sys.argv) == 5:
        bufsize = int(sys.argv[4])
        for _ in range(num_clients):
            spawn_client(filename, server_address, bufsize, output_data=results)
    else:
        for _ in range(num_clients):
            spawn_client(filename, server_address, output_data=results)

    print(f'Min time: {min(results["Times"])} ms')
    print(f'Max time: {max(results["Times"])} ms')
    print(f"Average time: {sum(results['Times']) / len(results['Times'])} ms")

    print()

    print(f'Min throughput: {min(results["Throughputs"])} KB/s')
    print(f'Max throughput: {max(results["Throughputs"])} KB/s')
    print(f"Average throughput: {sum(results['Throughputs']) / len(results['Throughputs'])} KB/s")

    sys.exit(0)

if __name__ == "__main__":
    main()
