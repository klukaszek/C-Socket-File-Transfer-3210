#include "../include/server.h"

int my_socket;            // socket used to listen for incoming connections
int con_socket;
int port_num;
int buf_size = 4096;	// default buffer size per assignment instructions

static void sigintCatcher(int signal __attribute__((unused)),  siginfo_t* si __attribute__((unused)), void *arg __attribute__((unused)))
{
    printf("\n\n************** Caught SIG_INT: shutting down the server ********************\n");

	close(con_socket);
	close(my_socket);
	exit(0);
}

int main(int argc, char *argv[])
{
	// Check for correct number of command line arguments
	if (argc < 2) {
        printf("Usage: %s <port-number> <optional: buffer size>\n", argv[0]);
        exit(1);
    }

	// Get port number and buffer size from command line arguments
    port_num = atoi(argv[1]);
	
	// Check if buffer size was specified
	if (argc == 3) buf_size = atoi(argv[2]);

	// Prevent using reserved ports
	if (port_num < 1024 || port_num > 65535) {
		printf("Port number must be between 1024 and 65535\n");
		exit(1);
	}

	// Prevent using buffer size of 0
	if (buf_size < 1) {
		printf("Buffer size must be greater than 0\n");
		exit(1);
	}

    printf("Port number: %d\n", port_num);
    printf("Buffer size: %d bytes\n", buf_size);
  
	struct sockaddr_in dest; // socket info about the machine connecting to us
	struct sockaddr_in serv; // socket info about our server

	// set up the sigint handler
	struct sigaction signaler;
    
    memset(&signaler, 0, sizeof(struct sigaction));
    sigemptyset(&signaler.sa_mask);

    signaler.sa_sigaction = sigintCatcher;
    signaler.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &signaler, NULL);

	//Set up socket info
	socklen_t socksize = sizeof(struct sockaddr_in);

	memset(&serv, 0, sizeof(serv));           // zero the struct before filling the fields
	
	serv.sin_family = AF_INET;                // Use the IPv4 address family
	serv.sin_addr.s_addr = htonl(INADDR_ANY); // Set our address to any interface 
	serv.sin_port = htons(port_num);           // Set the server port number 

	/* Create a socket.
   	   The arguments indicate that this is an IPv4, TCP socket
	*/
	my_socket = socket(AF_INET, SOCK_STREAM, 0);
  
	// bind serv information to mysocket
	// Unlike all other function calls in this example, this call to bind()
	// does some basic error handling

	int flag=1;

	if (setsockopt(my_socket,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag)) == -1) {
    	printf("setsockopt() failed\n");
		printf("%s\n", strerror(errno));
    	exit(1);
	} 

	if (bind(my_socket, (struct sockaddr *)&serv, sizeof(struct sockaddr)) != 0){
		printf("Unable to open TCP socket on localhost:%d\n", port_num);

	/* 	strerror() returns a string representation of the system variable "errno"
	 	errno is the integer code of the error that occured during the last system call from this process
	 	need to include errno.h to use this function
		*/
		printf("%s\n", strerror(errno));
		close(my_socket);
		return 0;
	}

	// start listening, allowing a queue of up to 1 pending connection
	listen(my_socket, 0);
	
	// Create a socket to communicate with the client that just connected
	con_socket = accept(my_socket, (struct sockaddr *)&dest, &socksize);
  
	while(con_socket)
	{
		//dest contains information - IP address, port number, etc. - in network byte order
		//We need to convert it to host byte order before displaying it
		printf("Incoming connection from %s on port %d\n", inet_ntoa(dest.sin_addr), ntohs(dest.sin_port));
		
		// Handle the file reception logic
		int result = receiveFile(con_socket, buf_size);

		// Check if there was an error receiving the file
		if (result == -1) {
			printf("Error receiving file from %s on port %d\n", inet_ntoa(dest.sin_addr), ntohs(dest.sin_port));
		}

		// Send the completion time to the client
		struct timespec completionTime;

		// Get the current time (use CLOCK_MONOTONIC to avoid issues with time changes)
		clock_gettime(CLOCK_MONOTONIC, &completionTime);

		// Send tv_sec
		if (send(con_socket, &completionTime.tv_sec, sizeof(time_t), 0) == -1) {
			perror("Failed to send completion time (sec)");
			return EXIT_FAILURE;
		}

		// Send tv_nsec
		if (send(con_socket, &completionTime.tv_nsec, sizeof(long), 0) == -1) {
			perror("Failed to send completion time (nsec)");
			return EXIT_FAILURE;
		}
		
		// Close current connection
		close(con_socket);

		// Continue listening for incoming connections
		con_socket = accept(my_socket, (struct sockaddr *)&dest, &socksize);
	}

	close(my_socket);
	return EXIT_SUCCESS;
}

// Function to receive a file from the client
int receiveFile(int socket, int size) 
{
	FILE *file = NULL;
	int filename_len;
	long file_size;
    long total_chunks;

	// Receive filename length
	if (recv(socket, &filename_len, sizeof(int), 0) == -1) {
		perror("\tFailed to receive filename length");
		return -1;
	}

	char filename[filename_len + 1];

    // Receive the filename
    if (recv(socket, filename, filename_len, 0) == -1) {
        perror("\tFailed to receive filename");
        return -1;
    }

	// Receive the file size
	if (recv(socket, &file_size, sizeof(long), 0) == -1) {
		perror("\tFailed to receive file size");
		return -1;
	}

	// Receive the total number of chunks
    if (recv(socket, &total_chunks, sizeof(long), 0) == -1) {
        perror("\tFailed to receive totalChunks");
        return -1;
    }

	// Null terminate the filename
	filename[filename_len] = '\0';

	// REMOVE THIS LATER
	char dir[256] = "files/";
	strcat(dir, filename);

    // Open the file for writing
    file = fopen(dir, "wb");
    if (file == NULL) {
        perror("\tFailed to create file");
        return -1;
    }

    printf("\tReceiving file: %s\n", filename);
    printf("\tTotal chunks: %ld\n", total_chunks);

	int total_bytes_received = 0;

    // Receive and write the file from the socket
    while (total_bytes_received < file_size)
	{
		char buffer[size];
	
		int bytes_read = recv(socket, buffer, size, 0);

		// Check if there was an error receiving the chunk
		if (bytes_read == -1)
		{
			perror("\tFailed to receive chunk");
			fclose(file);
			return -1;
		}

		// Update the total bytes received
		total_bytes_received += bytes_read;

		// Write the bytes to the file
		fwrite(buffer, 1, bytes_read, file);
	}

    printf("\tFile received successfully.\n");

    // Close the file
    fclose(file);

	return 0;
}