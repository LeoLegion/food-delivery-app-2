#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "client.h"
#include "../common/protocol.h"

int main() {
    int sock;
    struct sockaddr_in server;
    Request req;
    Response res;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    connect(sock, (struct sockaddr*)&server, sizeof(server));

    while (1) {
        printf("\n1.Register 2.Login 3.Exit\n");
        scanf("%d", &req.command);
        send(sock, &req, sizeof(req), 0);
        recv(sock, &res, sizeof(res), 0);

        printf("Server: %s\n", res.message);
    }

    close(sock);
}