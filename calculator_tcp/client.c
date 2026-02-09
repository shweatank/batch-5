#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUF_SIZE 1024

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Connected to Calculator Server\n");

    while (1) {
        printf("\nEnter calculation (e.g., 10 + 5): ");
        fgets(buffer, BUF_SIZE, stdin);

        send(sock, buffer, strlen(buffer), 0);

        memset(buffer, 0, BUF_SIZE);
        int n = recv(sock, buffer, BUF_SIZE, 0);
        if (n <= 0)
            break;

        printf("Server: %s\n", buffer);
    }

    close(sock);
    return 0;
}

