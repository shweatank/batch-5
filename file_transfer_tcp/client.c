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
    FILE *fp;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Connected to File Server\n");

    /* Enter file name */
    printf("Enter file name to send: ");
    scanf("%s", buffer);

    fp = fopen(buffer, "rb");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    /* Send file name */
    send(sock, buffer, strlen(buffer), 0);

    /* Send file data */
    int bytes;
    while ((bytes = fread(buffer, 1, BUF_SIZE, fp)) > 0) {
        send(sock, buffer, bytes, 0);
    }

    printf("File sent successfully\n");

    fclose(fp);
    close(sock);
    return 0;
}
