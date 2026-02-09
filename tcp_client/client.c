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

    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    /* Server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    /* Connect */
    if (connect(sock, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    printf("Connected to server.\n");

    /* Infinite send/receive loop */
    while (1) {
        /* Send message */
        snprintf(buffer, BUF_SIZE, "Hello from client");
        send(sock, buffer, strlen(buffer), 0);

        /* Receive response */
        memset(buffer, 0, BUF_SIZE);
        int n = recv(sock, buffer, BUF_SIZE, 0);
        if (n <= 0) {
            printf("Server disconnected\n");
            break;
        }

        printf("Server: %s\n", buffer);

        sleep(1);   // avoid flooding
    }

    close(sock);
    return 0;
}
