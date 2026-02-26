#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};

    /* 1️⃣ Create Socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket failed");
        return -1;
    }

    /* 2️⃣ Configure Address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "10.25.109.248", &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return -1;
    }

    /* 3️⃣ Connect */
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    printf("Connected to server.\n");

    /* 4️⃣ Send Data */
    char *message = "Hello from client";
    send(sock, message, strlen(message), 0);

    /* 5️⃣ Receive Response */
    read(sock, buffer, BUFFER_SIZE);
    printf("Server says: %s\n", buffer);

    close(sock);

    return 0;
}
