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
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;

    // Create socket
    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
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

    printf("Server listening on port %d\n", PORT);

    // Accept incoming connection
    socklen_t addr_size = sizeof(client_addr);
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size)) == -1)
    {
        perror("Acceptance failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    int num1, num2, answer, choice;


    // Communication loop
    while (1)
    {
        char *message1 = "Input Number 1: ";
        write(client_socket, message1, strlen(message1));
        read(client_socket, &num1, sizeof(int));
        printf("Client - Number 1 is: %d\n", num1);

        char *message2 = "Input Number 2: ";
        write(client_socket, message2, strlen(message2));
        read(client_socket, &num2, sizeof(int));
        printf("Client - Number 2 is: %d\n", num2);

        char *message3 = "Choose an operation:\n(1)Addition\n(2)Subtraction\n(3)Multiplication\n(4)Division\nChoice: ";
        write(client_socket, message3, strlen(message3));
        read(client_socket, &choice, sizeof(int));
        printf("Client - Choice is: %d\n", choice);

        switch(choice) {
            case 1:
                answer = num1 + num2;
                break;
            case 2:
                answer = num1 - num2;
                break;
            case 3:
                answer = num1 * num2;
                break;
            case 4:
                answer = num1 / num2;
                break;
            default:
                printf("s");
        }

        write(client_socket, &answer, sizeof(int));

        // ssize_t bytes_received = recv(client_socket, &buffer, sizeof(int), 0);
        // // Receive message from client
        // if (bytes_received <= 0)
        // {
        //     perror("Receive failed");
        //     break;
        // }

        // int new_num = 0;
        // new_num = buffer * buffer;

        // printf("Client sent: %d\n", buffer);
        // printf("Your number squared is: %d\n", new_num);

        // send(client_socket, &buffer, sizeof(int), 0);
    }

    close(server_socket);
    close(client_socket);

    return 0;
}