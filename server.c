#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  // Include this header for the close function

#define SERVER_PORT 8080  // Replace with the actual server port

void receive_data_ipv4_tcp() {
    int sockfd, new_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    char buffer[1024];

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Socket bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for incoming connections...\n");

    // Accept a client connection
    client_addr_len = sizeof(client_addr);
    new_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
    if (new_sockfd == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Connection accepted from: %s\n", inet_ntoa(client_addr.sin_addr));

    // Receive data from the client
    ssize_t bytes_received;
    size_t total_bytes_received = 0;
    while ((bytes_received = recv(new_sockfd, buffer, sizeof(buffer), 0)) > 0) {
        // Process received data
        printf("Received data of size: %zd\n", bytes_received);
        printf("Received data: %.*s\n", (int)bytes_received, buffer);
        
        // Update the total bytes received
        total_bytes_received += bytes_received;

        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));
    }

    if (bytes_received == -1) {
        perror("Data receive failed");
        exit(EXIT_FAILURE);
    }

    // Close the socket
    close(new_sockfd);
    close(sockfd);

    printf("Total bytes received: %zu\n", total_bytes_received);
}

void receive_data_ipv4_udp() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    char buffer[1024];

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Socket bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for incoming data...\n");

    // Receive data from the client
    ssize_t bytes_received;
    size_t total_bytes_received = 0;
    client_addr_len = sizeof(client_addr);
    bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_addr_len);
    if (bytes_received == -1) {
        perror("Data receive failed");
        exit(EXIT_FAILURE);
    }

    printf("Received data of size: %zd\n", bytes_received);
    printf("Received data: %.*s\n", (int)bytes_received, buffer);

    total_bytes_received += bytes_received;

    // Close the socket
    close(sockfd);

    printf("Total bytes received: %zu\n", total_bytes_received);
}

void receive_data_ipv6_tcp() {
    int sockfd, client_sockfd;
    struct sockaddr_in6 server_addr, client_addr;
    socklen_t client_len;
    char buffer[1024];

    // Create a TCP socket
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(SERVER_PORT);
    server_addr.sin6_addr = in6addr_any;  // Listen on all available interfaces

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 1) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", SERVER_PORT);

    // Accept the client connection
    client_len = sizeof(client_addr);
    client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
    if (client_sockfd == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    // Receive data from the client
    ssize_t bytes_received;
    while ((bytes_received = recv(client_sockfd, buffer, sizeof(buffer), 0)) > 0) {
        // Process the received data as needed
        // ...

        printf("Received data of size: %zd\n", bytes_received);
        printf("Data: %.*s\n", (int)bytes_received, buffer);
    }

    if (bytes_received == -1) {
        perror("Data receive failed");
        exit(EXIT_FAILURE);
    }

    // Close the client socket
    close(client_sockfd);

    // Close the server socket
    close(sockfd);
}

void receive_data_ipv6_udp() {
    int sockfd;
    struct sockaddr_in6 server_addr, client_addr;
    socklen_t client_len;
    char buffer[1024];

    // Create a UDP socket
    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(SERVER_PORT);
    server_addr.sin6_addr = in6addr_any;  // Listen on all available interfaces

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", SERVER_PORT);

    // Receive data from the client
    ssize_t bytes_received;
    client_len = sizeof(client_addr);
    bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);
    if (bytes_received == -1) {
        perror("Data receive failed");
        exit(EXIT_FAILURE);
    }

    printf("Received data of size: %zd\n", bytes_received);
    printf("Data: %.*s\n", (int)bytes_received, buffer);

    // Close the socket
    close(sockfd);
}

int main() {
   // receive_data_ipv4_tcp();
  //  receive_data_ipv4_udp();
    //receive_data_ipv6_tcp();
    receive_data_ipv6_udp();
    return 0;
}
