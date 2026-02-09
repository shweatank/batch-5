#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUF_SIZE 1024

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUF_SIZE];
    FILE *fp;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("File Server listening on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    printf("Client connected\n");

    /* Receive file name */
    recv(client_fd, buffer, BUF_SIZE, 0);
    printf("Receiving file: %s\n", buffer);

    fp = fopen(buffer, "wb");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    /* Receive file data */
    int bytes;
    while ((bytes = recv(client_fd, buffer, BUF_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes, fp);
    }

    printf("File received successfully\n");

    fclose(fp);
    close(client_fd);
    close(server_fd);

    return 0;
}
