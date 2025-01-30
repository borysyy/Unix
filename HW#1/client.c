#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create a socket for communication
    if ((client_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed"); // Display error if socket creation fails
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET; // IPv4 address family
    server_addr.sin_port = htons(PORT); // Set the port number
    
    // Set the IP address
    if(inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr)) <= 0)
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

    printf("Connected to server\n");
    int num1, num2, answer, choice;
    



    // Communication loop
    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
        read(client_socket, buffer, sizeof(buffer));
        printf("Server - %s", buffer);
        scanf("%d", &num1);
        write(client_socket, &num1, sizeof(int));

        memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
        read(client_socket, buffer, sizeof(buffer));
        printf("Server - %s", buffer);
        scanf("%d", &num2);
        write(client_socket, &num2, sizeof(int));

        memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
        read(client_socket, buffer, sizeof(buffer));
        printf("Server - %s", buffer);
        scanf("%d", &choice);
        write(client_socket, &choice, sizeof(int));

        read(client_socket, &answer, sizeof(int));
        printf("Server - The answer is: %d\n", answer);
    // //     printf("Client: ");
    // //     scanf("%d", &buffer);

    //     // Send message to server
    //     // send(client_socket, &buffer, sizeof(int), 0);

    //     // // memset(buffer, 0, BUFFER_SIZE); // Clear the buffer

    //     // // Receive message from server
    //     // ssize_t bytes_received = recv(client_socket, &buffer, sizeof(int), 0);

    //     // if (bytes_received <= 0)
    //     // {
    //     //     perror("Receive failed"); // Display error if receiving message fails
    //     //     exit(EXIT_FAILURE);
    //     // }

    //     // printf("Server: %d\n", buffer); // Display server's response
    }

    close(client_socket); // Close the socket connection

    return 0;
}