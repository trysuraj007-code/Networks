#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

int main() {
    int sock, N, a[10][10];
    struct sockaddr_in server;
    char result[50] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    printf("Enter N: ");
    scanf("%d", &N);

    if (N < 1 || N > 10) {
        printf("Invalid N\n");
        close(sock);
        return 1;
    }

    printf("Enter matrix:\n");
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            scanf("%d", &a[i][j]);

    send(sock, &N, sizeof(N), 0);
    send(sock, a, N * N * sizeof(int), 0);

    recv(sock, result, sizeof(result) - 1, 0);
    printf("Matrix Type: %s\n", result);

    close(sock);
    return 0;
}