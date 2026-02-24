#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main()
{
    int server_fd, client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};

    /* 1️⃣ Create Socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        return -1;
    }

    /* 2️⃣ Configure Address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    /* 3️⃣ Bind */
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return -1;
    }

    /* 4️⃣ Listen */
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        return -1;
    }

    printf("Server listening on port %d...\n", PORT);

    /* 5️⃣ Accept Connection */
    client_socket = accept(server_fd, NULL, NULL);
    if (client_socket < 0) {
        perror("Accept failed");
        return -1;
    }

    printf("Client connected.\n");

    /* 6️⃣ Receive Data */
    read(client_socket, buffer, BUFFER_SIZE);
    printf("Client says: %s\n", buffer);

    /* 7️⃣ Send Response */
    char *response = "Hello from server";
    send(client_socket, response, strlen(response), 0);

    close(client_socket);
    close(server_fd);

    return 0;
}
