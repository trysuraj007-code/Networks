#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};
    char *msg = "Hello from client";

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    send(sock, msg, strlen(msg), 0);
    printf("Message sent to server\n");

    read(sock, buffer, sizeof(buffer) - 1);
    printf("Server: %s\n", buffer);

    close(sock);

    return 0;
}