//This is the server program that will accept client connections and perform math operations based on the client requests.
//TCP server that performs basic math operations (+, -, *, /) based on the client requests
//Used TCP because it is connection-oriented and reliable , suitable for real-time applications like this one.
//Didn't use UDP because it is connectionless and unreliable, not suitable for real-time applications.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  //POSIX API for system calls (close, read, write, etc.)
#include <arpa/inet.h> //Used for defining internet operations (inet_pton, sockaddr, etc.)
#include <sys/socket.h>  //Socket API
#include <netinet/in.h>  //Internet address family (struct sockaddr_in)

#define BUFFER_SIZE 1024 //This defines the max buffer size for the messages

//Function prototype that will send messages to the client
void send_message(int, const char*);

int main(int argc, char *argv[])
{
    //Check if the user has entered the correct number of arguments (PORT number)
    if(argc != 2)
    {
        //If the user has not entered the correct number of arguments, print the usage and return 1
        printf("USAGE: ./server PORT#\n");
        return 1;
    }

    int server_socket, client_socket;   //Server and client socket file descriptors
    struct sockaddr_in server_addr, client_addr; //Server and client address structures
    char buffer[BUFFER_SIZE];  //Buffer to store the messages

    //Create a TCP socket for the server (Beej's Guide: Section 5.2 - "socket()")
    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        //If the socket creation fails, print an error message and exit the program
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Remove the "address is in use" message when you restart the server (Beej's Guide: Section 5.2 - "setsockopt()")
    int yes = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        //If the setsockopt fails, print an error message and exit the program
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    int port = atoi(argv[1]); // Convert the port number from string to integer

    //Set up the server address structure
    server_addr.sin_family = AF_INET;  //Set the address family to IPv4
    server_addr.sin_port = htons(port);  //Conbert the port number to network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY; //Bind to all available network interfaces

    //Bind the socket to the specified address and port (Beej's Guide: Section 5.2 - "bind()")
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }
    //Listen for incoming client connections on the server socket (Beej's Guide: Section 5.2 - "listen()")
    if (listen(server_socket, 5) == -1)
    {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    //Print the port number the server is listening on
    printf("Server listening on port %d\n", port);

    //Main server loop
    //Accept incoming client connections
    while (1)
    {
        //Accept a client connection and create a new socket for the client (Beej's Guide: Section 5.2 - "accept()")
        socklen_t addr_size = sizeof(client_addr);
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size)) == -1)
        {
            perror("Acceptance failed");
            continue;
        }

        printf("Client connected\n");

        //Main client loop
        //Handles client requests
        while(1)
        {
            //Receive data from the client
            int num1, num2, answer = 0;
            char operator;

            //Send a message to the client "Enter math problem or \"exit\" to leave: "
            send_message(client_socket, "Enter math problem or \"exit\" to leave: ");

            memset(buffer, 0, BUFFER_SIZE); //Clear the buffer
            ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0); //Receive data from the client (Beej's Guide: Section 6.2 - "recv()")

            buffer[bytes_received] = '\0';  //Null terminate the buffer

            //Check if the client wants to exit
            if(strcmp(buffer, "exit") == 0)
            {
                //If the client wants to exit, close the client socket and break out of the client loop
                printf("Closing client socket\n");
                close(client_socket);
                break;
            }

            printf("Client - %s\n", buffer); //Print the client message
            //Parse the client message for the math problem
            if(sscanf(buffer, "%d %c %d", &num1, &operator, &num2) == 3)
            {   
                //Perform the math operation based on the operator and the numbers provided
                switch (operator)
                {
                case '+': 
                    answer = num1 + num2; 
                    break;
                case '-': 
                    answer = num1 - num2; 
                    break;
                case '*': 
                    answer = num1 * num2; 
                    break;
                case '/':
                    if(num2 == 0)
                    {   
                        //Check for division by zero
                        send_message(client_socket, "Error: Division by zero!");
                        continue;
                    }
                    answer = num1 / num2;
                    break;
                default:
                    //If the operator is not recognized, send an error message to the client
                    send_message(client_socket, "Error: Invalid operator!");
                    continue;
                }
            }
            else
            {   //If the client message is not in the correct format, send an error message to the client
                send_message(client_socket, "Error: Invalid input format!");
                continue;
            }

            char message[BUFFER_SIZE]; //Buffer to store the message to be sent to the client
            // Create a custom message for the client
            // buffer: 1 + 1
            // answer: 2
            // message: 1 + 1 = 2
            snprintf(message, sizeof(message), "%s = %d", buffer, answer);
            send_message(client_socket, message);
        }
    }
    //Close the server socket
    close(server_socket);
    return 0;
}
//Function to send messages to the client socket
void send_message(int client_socket, const char* message)
{   
    ssize_t total_sent = 0; //Total bytes sent to the client
    ssize_t bytes_to_send = strlen(message); //Bytes to send to the client
    //Send the message to the client in a loop until all bytes are sent
    while (total_sent < bytes_to_send) {
        // Send the message to the client
        ssize_t sent = send(client_socket, message + total_sent, bytes_to_send - total_sent, 0);
        // Check if the send failed
        if (sent == -1) {
            perror("send failed");
            return;
        }
        total_sent += sent;
    }

    // Wait for the client to acknowledge receipt before sending the next message 
    char ack[3];
    ssize_t bytes_received = recv(client_socket, ack, sizeof(ack), 0); // Waiting for acknowledgment from the client
    ack[bytes_received] = '\0';
    // Check if the client acknowledged the message
    if (bytes_received <= 0) {
        printf("Client did not acknowledge the message\n");
        return;
    }

    printf("Client acknowledged: %s\n", ack);
}

