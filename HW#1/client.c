//This is the client.c file. The client.c file is responsible for connecting to the server and sending and receiving messages. 
//The client.c file is similar to the server.c file, but it does not have the logic to perform calculations.
//TCP client that connects to a server and sends and receives messages
//Used TCP because it is a reliable connection-oriented protocol suitable for real-time applications like this one.
//Didn't use UDP because it is a connectionless and unreliable protocol, not suitable for real-time applications. 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //POSIX API for system calls (close, read, write, etc.)
#include <arpa/inet.h> //Used for defining internet operations (inet_pton, sockaddr, etc.)
#include <sys/socket.h> //Socket API
#include <netinet/in.h> //Internet address family (struct sockaddr_in)

#define BUFFER_SIZE 1024  //Max buffer size for messages

ssize_t recv_message(int, char *, size_t); //Function prototype to receive messages from the server

//Main function that connects to the server and sends and receives messages
int main(int argc, char *argv[])
{
    //Check if the user has entered the correct number of arguments (IP address and PORT number)
    if(argc != 3)
    {
        printf("USAGE: ./client IP_ADDRESS PORT#\n");
        return 1;
    }

    int client_socket;  //Client socket file descriptor
    struct sockaddr_in server_addr; //Server address structure to connect to the server
    char buffer[BUFFER_SIZE];  //Buffer to store the messages
    //Create a TCP socket for the client to connect to the server (Beej's Guide: Section 5.2 - "socket()")
    if ((client_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        //If the socket creation fails, print an error message and exit the program
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    char* ip_address = argv[1]; //IP address of the server
    int port = atoi(argv[2]);  //Port number of the server

    server_addr.sin_family = AF_INET;  //Set the address family to IPv4
    server_addr.sin_port = htons(port);  //Convert the port number to network byte order
    //Convert the IP address from string to binary form and store it in the server address structure
    //Beej's Guide: Section 5.3 - "inet_pton()"
    if(inet_pton(AF_INET, ip_address, &(server_addr.sin_addr)) <= 0)
    {
        //If the IP address conversion fails, print an error message and exit the program
        printf("Invalid Address\n");
        exit(EXIT_FAILURE);
    }
    //Connect to the server using the client socket and server address structure (Beej's Guide: Section 5.2 - "connect()")
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");  //Print a message indicating that the client is connected to the server
    printf("Example math problem: 1 + 1\n"); //Print an example math problem for the user
    printf("Allowed operators: + , - , * , /\n"); //Print the allowed operators for the math problems
    //Main client loop
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE); //Clear the buffer to store the user's input
        ssize_t bytes_received = recv_message(client_socket, buffer, BUFFER_SIZE); //Receive a message from the server and store it in the buffer
        buffer[bytes_received] = '\0';

        printf("\nServer - %s", buffer); //Print the message received from the server 
        fgets(buffer, sizeof(buffer), stdin); //Get user input from the console
        buffer[strcspn(buffer, "\n")] = 0; //Remove the newline character from the user input

        if(send(client_socket, buffer, strlen(buffer), 0) == -1)
        {
            perror("send failed");
            exit(EXIT_FAILURE);
        }

        if(strcmp(buffer, "exit") == 0)
        {
            //If the user wants to exit, close the client socket and exit the program
            printf("Closing connection\n");
            close(client_socket);
            exit(EXIT_SUCCESS);
        }

        memset(buffer, 0, BUFFER_SIZE); //Clear the buffer to store the message received from the server
        bytes_received = recv_message(client_socket, buffer, BUFFER_SIZE); //Receive a message from the server and store it in the buffer

        buffer[bytes_received] = '\0'; //Null-terminate the buffer to prevent buffer overflow
        printf("Server - %s\n", buffer); //Print the message received from the server
    }

    return 0;
}
//Function to receive messages from the server (Beej's Guide: Section 6.2 - "recv()")
ssize_t recv_message(int client_socket, char *buffer, size_t buffer_size) 
{
    ssize_t bytes_received = recv(client_socket, buffer, buffer_size - 1, 0); //Receive a message from the server and store it in the buffer
   
    if (bytes_received == -1) //Check if an error occurred during the receive operation
    {
        perror("recv failed");
        exit(EXIT_FAILURE);
    }

    // Send acknowledgment back to the server (Beej's Guide: Section 6.2 - "send()")
    const char *ack = "ACK"; 
    if(send(client_socket, ack, strlen(ack), 0) == -1)
    {
        perror("send failed");
        exit(EXIT_FAILURE);
    }

    return bytes_received;
}

