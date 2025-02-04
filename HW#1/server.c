#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

void send_message(int, const char*);

#define BUFFER_SIZE 256

int main(int argc, char *argv[])
{
    // Check if argument count is just 1
    if(argc == 1)
    {
        printf("USAGE: ./server PORT#");
        return 1;
    }

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    // Create a socket
    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Convert port string to integer
    int port = atoi(argv[1]);

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1)
    {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    // Server is listening message
    printf("Server listening on port %d\n", port);

    // Accepting loop
    while (1)
    {
        // Accept incoming connection
        socklen_t addr_size = sizeof(client_addr);
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size)) == -1)
        {
            perror("Acceptance failed");
            exit(EXIT_FAILURE);
        }

        printf("Client connected\n");
    
        // Communication loop
        while(1)
        {
            int num1, num2, answer = 0;
            char operator;

            // Wait before sending message between inputs
            sleep(1);

            // Message for client to submit input
            send_message(client_socket, "Enter math problem or \"exit\" to leave: ");

            // Clear the buffer
            memset(buffer, 0, BUFFER_SIZE);
            recv(client_socket, buffer, sizeof(buffer), 0);

            // Check if the client exited 
            if(strcmp(buffer, "exit") == 0)
            {
               printf("Closing client socket\n");
               close(client_socket);
               break;
            }

            printf("Client - %s\n", buffer);

            // Check if the user inputted the correct format string
            // Ex. 1 + 1
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
                    if(num2 != 0)
                    {
                        answer = num1 / num2;
                    }
                    else 
                    {
                        send_message(client_socket, "Error: Dividing by zero!");
                        continue;
                    }
                    break;
                // If client inputted incorrect operator then send error
                default:
                    send_message(client_socket, "Error: Invalid operator!");
                    continue;
                }
              
            }
            // If client inputted incorrect string format then send error
            else
            {
                send_message(client_socket, "Error: Invalid input format!");
                continue;
            }


            // Send the client a string formatted as:
            // 1 + 1 = 2
            // buffer - Initial client input
            // answer - Answer to client's input
            char message[256];
            sprintf(message, "%s = %d", buffer, answer);
            send_message(client_socket, message);
        }

    }

    close(server_socket);

    return 0;
}

void send_message(int client_socket, const char* message)
{
    send(client_socket, message, strlen(message), 0);
}