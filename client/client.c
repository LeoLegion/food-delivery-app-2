#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
// #include "client.h"
// #include "../common/protocol.h"

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server;
    // Request req;
    // Response res;
    char buffer[256];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    connect(sock, (struct sockaddr*)&server, sizeof(server));

    printf("Connected to server. Type messages (type 'exit' to quit)\n");

    while (1) {
        // printf("\n1.Register 2.Login 3.Exit\n");
        // scanf("%d", &req.command);
        // send(sock, &req, sizeof(req), 0);
        // recv(sock, &res, sizeof(res), 0);

        // printf("Server: %s\n", res.message);
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);

        if (strncmp(buffer, "exit", 4) == 0)
            break;

        send(sock, buffer, strlen(buffer), 0);

        int n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        buffer[n] = '\0';

        printf("Server: %s", buffer);
    }

    close(sock);
    return 0;
}