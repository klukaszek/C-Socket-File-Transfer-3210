# CIS 3210 - Assignment 1
# Author: Kyle Lukaszek
# ID: 1113798
# Due: October 13, 2023

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Iinclude

# Desired executable paths
SERVER_EXEC = bin/server
CLIENT_EXEC = bin/sendFile

# Source and header file paths for server and client
SERVER_SRC = src/server.c
CLIENT_SRC = src/sendFile.c

# Targets
all: server client

server: $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_EXEC) $^

client: $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_EXEC) $^

clean:
	rm -f $(SERVER_EXEC) $(CLIENT_EXEC)