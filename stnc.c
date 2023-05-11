#include "stnc.h"

int main(int argc, char *argv[])
{

	if (argc < 3)
	{

		printf("Usage : PartA:\n   Client: ./stnc -[c] <ip> <port>\n Server: ./stnc -[s] <port>\n");
		printf("Usage : PartB:\n   Client: ./stnc -[c] <ip> <port> <file>\n Server: ./stnc -[s] <port> <file>\n");
		return 1;
	}
	if (!strcmp(argv[1], "-c"))
	{
		handle_client(argv[2], argv[3]);
	}
	else if (!strcmp(argv[1], "-s"))
	{
		handle_server(argv[2]);
	}
	else
	{
		printf("Usage : PartA:\n   Client: ./stnc -[c] <ip> <port>\n Server: ./stnc -[s] <port>\n");
		printf("Usage : PartB:\n   Client: ./stnc -[c] <ip> <port> <file>\n Server: ./stnc -[s] <port> <file>\n");
		return 1;
	}
	return 0;
}

void handle_client(char *ip, char *port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		printf("Error creating socket\n");
		exit(1);
	}
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(port));

	if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
	{
		printf("Invalid address/ Address not supported\n");
		exit(1);
	}
	if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("Connection Failed\n");
		exit(1);
	}
	printf("Connected to %s:%s\n", ip, port);
	char buffer[BUFFER_SIZE];
	struct pollfd fds[2];
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;
	fds[1].fd = sock;
	fds[1].events = POLLIN;
	while (true)
	{
		int ret = poll(fds, 2, -1);
		if (ret < 0)
		{
			printf("ERROR poll() failed\n");
			exit(1);
		}

		if (fds[0].revents & POLLIN)
		{
			// Read user input
			int bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
			if (bytes_read < 0)
			{
				printf("ERROR read() failed\n");
				exit(1);
			}
			buffer[bytes_read] = '\0';

			// Send user input to server
			int bytes_sent = send(sock, buffer, bytes_read, 0);
			if (bytes_sent < 0)
			{
				printf("ERROR send() failed\n");
				exit(1);
			}
			bzero(buffer, BUFFER_SIZE);
		}
		if (fds[1].revents & POLLIN) // check if server sent message
		{
			// Read message from server
			int bytes_recieved = recv(sock, buffer, BUFFER_SIZE - 1, 0);
			if (bytes_recieved < 0)
			{
				printf("ERROR recv() failed\n");
				exit(1);
			}
			if (bytes_recieved == 0)
			{
				printf("Server disconnected\n");
				exit(1);
			}
			buffer[bytes_recieved] = '\0';
			printf("Server: %s", buffer);
			bzero(buffer, BUFFER_SIZE);
		}
	}
	close(sock);
}

void handle_server(char *port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		printf("ERROR opening socket\n");
		exit(1);
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(port));
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("ERROR on binding\n");
		exit(1);
	}
	printf("Starting chat server on port %s\n", port);
	if (listen(sock, 1) < 0)
	{
		printf("ERROR on listen\n");
		exit(1);
	}
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	while (true)
	{
		int client_sock = accept(sock, (struct sockaddr *)&client_addr, &client_len);
		if (client_sock < 0)
		{
			printf("ERROR on accept\n");
			exit(1);
		}
		printf("Client connected  \n");

		struct pollfd fds[2];

		fds[0].fd = STDERR_FILENO;
		fds[0].events = POLLIN;
		fds[1].fd = client_sock;
		fds[1].events = POLLIN;
		char buffer[BUFFER_SIZE];
		while (true)
		{
			int ret = poll(fds, 2, -1);
			if (ret < 0)
			{
				printf("ERROR poll() failed\n");
				exit(1);
			}
			if (fds[0].revents & POLLIN) // check if user input
			{
				// Read user input
				int bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
				if (bytes_read < 0)
				{
					printf("ERROR read() failed\n");
					exit(1);
				}
				buffer[bytes_read] = '\0';
				if (send(client_sock, buffer, bytes_read, 0) < 0)
				{
					printf("ERROR send() failed\n");
					exit(1);
				}
				bzero(buffer, BUFFER_SIZE);
			}
			if (fds[1].revents & POLLIN)
			{
				// Read message from client
				int bytesRecv = recv(client_sock, buffer, BUFFER_SIZE - 1, 0);
				if (bytesRecv < 0)
				{
					printf("ERROR recv() failed\n");
					exit(1);
				}
				if (bytesRecv == 0)
				{
					printf("Client disconnected\n");
					break;
				}
				buffer[bytesRecv] = '\0';
				printf("Client: %s", buffer);
				bzero(buffer, BUFFER_SIZE);
			}

		}
		close(client_sock);
	}

	close(sock);
}
