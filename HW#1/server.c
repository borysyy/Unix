#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

void send_message(int, const char*);

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("USAGE: ./server PORT#\n");
        return 1;
    }

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Remove the "address is in use" message when you restart the server
    int yes = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) == -1)
    {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", port);

    while (1)
    {
        socklen_t addr_size = sizeof(client_addr);
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size)) == -1)
        {
            perror("Acceptance failed");
            continue;
        }

        printf("Client connected\n");

        while(1)
        {
            int num1, num2, answer = 0;
            char operator;

            send_message(client_socket, "Enter math problem or \"exit\" to leave: ");

            memset(buffer, 0, BUFFER_SIZE);
            ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

            buffer[bytes_received] = '\0'; 

            if(strcmp(buffer, "exit") == 0)
            {
                printf("Closing client socket\n");
                close(client_socket);
                break;
            }

            printf("Client - %s\n", buffer);

            if(sscanf(buffer, "%d %c %d", &num1, &operator, &num2) == 3)
            {
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
                        send_message(client_socket, "Error: Division by zero!");
                        continue;
                    }
                    answer = num1 / num2;
                    break;
                default:
                    send_message(client_socket, "Error: Invalid operator!");
                    continue;
                }
            }
            else
            {
                send_message(client_socket, "Error: Invalid input format!");
                continue;
            }

            char message[BUFFER_SIZE];
            // Create a custom message for the client
            // buffer: 1 + 1
            // answer: 2
            // message: 1 + 1 = 2
            snprintf(message, sizeof(message), "%s = %d", buffer, answer);
            send_message(client_socket, message);
        }
    }

    close(server_socket);
    return 0;
}

void send_message(int client_socket, const char* message)
{
    if ( send(client_socket, message, strlen(message), 0) == -1) {
        perror("send failed");
        return;
    }
    
    // Wait for the client to acknowledge receipt before sending the next message
    char ack[3];
    ssize_t bytes_received = recv(client_socket, ack, sizeof(ack), 0); // Waiting for acknowledgment
    ack[bytes_received] = '\0';
    
    if (bytes_received <= 0) {
        printf("Client did not acknowledge the message\n");
        return;
    }

    printf("Client acknowledged: %s\n", ack);
}

