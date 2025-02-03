#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

void send_message(int client_socket, const char* message)
{
    send(client_socket, message, strlen(message), 0);
}


int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        printf("USAGE: ./server PORT#");
        return 1;
    }

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

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
            char operator, flag;

            sleep(1);
            
            send_message(client_socket, "Enter math problem or \"exit\" to leave: ");
            memset(buffer, 0, BUFFER_SIZE);
            recv(client_socket, buffer, sizeof(buffer), 0);

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
                    if(num2 != 0)
                    {
                        answer = num1 / num2;
                    }
                    else 
                    {
                        flag = 'E';
                        send(client_socket, &flag, sizeof(char), 0);
                        send_message(client_socket, "Error: Dividing by zero!");
                        continue;
                    }
                    break;
                   
                default:
                    flag = 'E';
                    send(client_socket, &flag, sizeof(char), 0);
                    send_message(client_socket, "Error: Invalid operator!");
                    continue;
                }
              
            }
            else
            {
                flag = 'E';
                send(client_socket, &flag, sizeof(char), 0);
                send_message(client_socket, "Error: Invalid input format!");
                continue;
            }

            flag = 'R';
            send(client_socket, &flag, sizeof(char), 0);
            send(client_socket, &answer, sizeof(int), 0);

        }

    }

    close(server_socket);

    return 0;
}