#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>

// #include "server.h"

#define PORT 8080

#define MAX_USERS 100

typedef struct {
    char username[50];
    char password[50];
} User;

User users[MAX_USERS];
int user_count = 0;

pthread_mutex_t user_mutex = PTHREAD_MUTEX_INITIALIZER;

// int server_fd;
// pthread_mutex_t file_mutex;

int register_user(const char *username, const char *password);

void *handle_client(void *arg);

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // pthread_mutex_init(&file_mutex, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socker failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server running on port %d...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);

        pthread_t tid;
        int* pclient = malloc(sizeof(int));
        *pclient = client_fd;

        pthread_create(&tid, NULL, handle_client, pclient);
        pthread_detach(tid);
    }

    return 0;
}

int register_user(const char *username, const char *password) {
    pthread_mutex_lock(&user_mutex);
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            pthread_mutex_unlock(&user_mutex);
            return 0;
        }
    }

    if (user_count >= MAX_USERS) {
        pthread_mutex_unlock(&user_mutex);
        return -1;
    }

    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password, password);

    pthread_mutex_unlock(&user_mutex);
    return 1;
}

void *handle_client(void *arg) {
    int client_fd = *((int *)arg);
    free(arg);

    char buffer[256];

    while (1) {
        int n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0)
            break;

        buffer[n] = '\0';
        printf("Client says: %s\n", buffer);

        char command[20], username[50], password[50];

        sscanf(buffer, "%s %s %s", command, username, password);

        if (strcmp(command, "REGISTER") == 0) {
            int result = register_user(username, password);

            if (result == 1) {
                send(client_fd, "REGISTER_SUCCESS\n", 17, 0);
            } else if (result == 0) {
                send(client_fd, "USER_ALREADY_EXISTS\n", 20, 0);
            } else {
                send(client_fd, "SERVER_FULL\n", 12, 0);
            }
        } else {
            send(client_fd, "INVALID_COMMAND\n", 16, 0);
        }
            }

    close(client_fd);
    return NULL;
}