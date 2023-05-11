#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>  // Include this header for the close function

#define SERVER_IP "127.0.0.1"  // Replace with the actual server IP
#define SERVER_IPV6 "fe80::b9d4:2fa4:1d80:de98/64"  // IPv4-mapped IPv6 address
#define SERVER_PORT 8080      // Replace with the actual server port

void send_data_ipv4_tcp(const void* data, size_t data_size) {
    int sockfd;
    struct sockaddr_in server_addr;

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(server_addr.sin_addr)) <= 0) {
        perror("Invalid server IP address");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Send the data
    ssize_t bytes_sent = send(sockfd, data, data_size, 0);
    if (bytes_sent == -1) {
        perror("Data send failed");
        exit(EXIT_FAILURE);
    }

    printf("Sent data of size: %zu\n", data_size);

    // Close the socket
    close(sockfd);
}

//////////////////////////////////////////////////////////////////////////////////////////////
void send_data_ipv4_udp(const void* data, size_t data_size) {
    int sockfd;
    struct sockaddr_in server_addr;

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(server_addr.sin_addr)) <= 0) {
        perror("Invalid server IP address");
        exit(EXIT_FAILURE);
    }

    // Send the data
    ssize_t bytes_sent = sendto(sockfd, data, data_size, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (bytes_sent == -1) {
        perror("Data send failed");
        exit(EXIT_FAILURE);
    }

    printf("Sent data of size: %zu\n", data_size);

    // Close the socket
    close(sockfd);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void send_data_ipv6_tcp(const void* data, size_t data_size) {
    int sockfd;
    struct sockaddr_in6 server_addr;

    // Create a TCP socket
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, SERVER_IPV6, &(server_addr.sin6_addr));
    server_addr.sin6_port = htons(SERVER_PORT);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Send the data
    ssize_t bytes_sent = send(sockfd, data, data_size, 0);
    if (bytes_sent == -1) {
        perror("Data send failed");
        exit(EXIT_FAILURE);
    }

    printf("Sent data of size: %zu\n", data_size);

    // Close the socket
    close(sockfd);
}

///////////////////////////////////////////////////////////////////////////////////
void send_data_ipv6_udp(const void* data, size_t data_size) {
    int sockfd;
    struct sockaddr_in6 server_addr;

    // Create a UDP socket
    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET6, "::1", &(server_addr.sin6_addr)) != 1) {
        perror("Invalid server IP address");
        exit(EXIT_FAILURE);
    }

    // Send the data
    ssize_t bytes_sent = sendto(sockfd, data, data_size, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (bytes_sent == -1) {
        perror("Data send failed");
        exit(EXIT_FAILURE);
    }

    printf("Sent data of size: %zu\n", data_size);

    // Close the socket
    close(sockfd);
}

////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char* argv[]) {
    // printf("argc - %d\n", argc);
    // for(int i = 0; i < argc; i++){
    //     printf("argv - %s\n", argv[i]);
    // }

    if (argc != 4) {
        printf("Invalid arguments. Usage: %s -p <type> <param>\n", argv[0]);
        return 1;
    }

    int perform_test = 0;
    char* type = NULL;
    char* param = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            perform_test = 1;
        } else if (type == NULL) {
            type = argv[i];
        } else if (param == NULL) {
            param = argv[i];
        }
    }

    if (!perform_test) {
        printf("No test flag (-p) specified.\n");
        return 1;
    }

    if (type == NULL || param == NULL) {
        printf("Invalid arguments. Usage: %s -p <type> <param>\n", argv[0]);
        return 1;
    }
       FILE* file = fopen("evyatar.txt", "rb");
        if (file == NULL) {
            perror("File open failed");
            return 1;
        }

        // Get the file size
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Read the file contents into a buffer
               // Read the file contents into a buffer
        char* buffer = (char*)malloc(file_size);
        if (buffer == NULL) {
            perror("Memory allocation failed");
            fclose(file);
            return 1;
        }

        size_t bytes_read = fread(buffer, 1, file_size, file);
        if (bytes_read != file_size) {
            perror("File read failed");
            fclose(file);
            free(buffer);
            return 1;
        }

        fclose(file);

    // Perform the test based on the specified type and param
    if (strcmp(type, "ipv4") == 0 ) {
         if(strcmp(param, "tcp")==0){
        
        printf("Performing IPv4 test with param: %s\n", param);
   
        // Send the file contents over IPv4 TCP
        send_data_ipv4_tcp(buffer, bytes_read);

        // Free the buffer
        free(buffer);
        }
        else if(strcmp(param, "udp")==0)
        {

         printf("Performing IPv4 test with param: %s\n", param);
         send_data_ipv4_udp(buffer, bytes_read);

        }
    } else if (strcmp(type, "ipv6") == 0) {
         if(strcmp(param, "tcp")==0){
              send_data_ipv6_tcp(buffer, bytes_read);
              printf("Performing IPv6 test with param: %s\n", param);
         }
         else if(strcmp(param, "udp") == 0)
        send_data_ipv6_udp(buffer,bytes_read);
        printf("Performing IPv6 test with param: %s\n", param);
        // Implement the necessary actions for IPv6 file transmissionl
    } else if (strcmp(type, "mmap") == 0) {
        // Test for mmap communication
        // param can be file name
        // Perform the appropriate actions
        printf("Performing mmap test with param: %s\n", param);
        // Implement the necessary actions for mmap file transmission
    } else if (strcmp(type, "pipe") == 0) {
        // Test for pipe communication
        // param can be any additional required information
        // Perform the appropriate actions
        printf("Performing pipe test with param: %s\n", param);
        // Implement the necessary actions for pipe file transmission
    } else if (strcmp(type, "uds") == 0) {
        // Test for Unix Domain Socket communication
        // param can be any additional required information
        // Perform the appropriate actions
        printf("Performing Unix Domain Socket test with param: %s\n", param);
        // Implement the necessary actions for Unix Domain Socket file transmission
    } else {
        printf("Invalid communication type: %s\n", type);
        return 1;
    }

    return 0;
}

