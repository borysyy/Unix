#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

void recv_message(int, char *, size_t);

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("USAGE: ./client IP_ADDRESS PORT#\n");
        return 1;
    }

    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    if ((client_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    char* ip_address = argv[1];
    int port = atoi(argv[2]);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if(inet_pton(AF_INET, ip_address, &(server_addr.sin_addr)) <= 0)
    {
        printf("Invalid Address\n");
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");
    printf("Example math problem: 1 + 1\n");
    printf("Allowed operators: + , - , * , /\n");

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        recv_message(client_socket, buffer, BUFFER_SIZE);

        printf("\nServer - %s", buffer);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        send(client_socket, buffer, strlen(buffer), 0);

        if(strcmp(buffer, "exit") == 0)
        {
            printf("Closing connection\n");
            close(client_socket);
            exit(EXIT_SUCCESS);
        }

        memset(buffer, 0, BUFFER_SIZE);
        recv_message(client_socket, buffer, BUFFER_SIZE);

        printf("Server - %s\n", buffer);
    }

    return 0;
}

void recv_message(int client_socket, char *buffer, size_t buffer_size) 
{
    ssize_t bytes_received = recv(client_socket, buffer, buffer_size - 1, 0);

    if (bytes_received == 0) 
    {
        printf("Connection closed by peer\n");
    } 
    else if (bytes_received == -1) 
    {
        perror("recv failed");
        exit(EXIT_FAILURE);
    }

    // Send acknowledgment back to the server
    const char *ack = "ACK"; 
    send(client_socket, ack, strlen(ack), 0);

}

