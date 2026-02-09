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

    printf("Calculator Server listening on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    printf("Client connected\n");

    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int n = recv(client_fd, buffer, BUF_SIZE, 0);
        if (n <= 0)
            break;

        double a, b, result;
        char op;
        char response[BUF_SIZE];

        sscanf(buffer, "%lf %c %lf", &a, &op, &b);

        switch (op) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/':
                if (b == 0) {
                    snprintf(response, BUF_SIZE, "Error: Division by zero");
                    send(client_fd, response, strlen(response), 0);
                    continue;
                }
                result = a / b;
                break;
            default:
                snprintf(response, BUF_SIZE, "Invalid operator");
                send(client_fd, response, strlen(response), 0);
                continue;
        }

        snprintf(response, BUF_SIZE, "Result = %.2lf", result);
        send(client_fd, response, strlen(response), 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
