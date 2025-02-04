#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    // Check if argument count is just 1
    if(argc == 1)
    {
        printf("USAGE: ./client IP_ADDRESS PORT#");
        return 1;
    }

    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create a socket 
    if ((client_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed"); 
        exit(EXIT_FAILURE);
    }

    // IP Address and convert port string to integer
    char* ip_address = argv[1];
    int port = atoi(argv[2]);

    // Configure server address
    server_addr.sin_family = AF_INET; // IPv4 address family
    server_addr.sin_port = htons(port); // Set the port number
    
    // Set the IP address
    if(inet_pton(AF_INET, ip_address, &(server_addr.sin_addr)) <= 0)
    {
        printf("Invalid Address\n");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connection failed"); // Display error if connection fails
        exit(EXIT_FAILURE);
    }

    // Initial messages
    printf("Connected to server\n");
    printf("Example math problem: 1 + 1\n");
    printf("Allowed operators: + , - , * , /\n");

    // Communication loop
    while (1)
    {
        // Clear the buffer
        memset(buffer, 0, BUFFER_SIZE);
        // Receive server's inital message
        recv(client_socket, buffer, BUFFER_SIZE, 0);
        printf("\nServer - %s", buffer);
        
        // Get the math problem from user
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        // Send to server
        send(client_socket, buffer, strlen(buffer), 0);

        // Check if the client wants to exit
        if(strcmp(buffer, "exit") == 0)
        {
            printf("Closing connection\n");
            close(client_socket); 
            exit(EXIT_SUCCESS);
        }

        memset(buffer, 0, BUFFER_SIZE);
        // Receive the server's response to client's input
        recv(client_socket, buffer, BUFFER_SIZE, 0);
        printf("Server - %s\n", buffer);

    }


    return 0;
}