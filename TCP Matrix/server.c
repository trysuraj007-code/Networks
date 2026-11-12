#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

int main() {
    int s, c, N, a[10][10];
    struct sockaddr_in server;
    char result[50];

    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(s, (struct sockaddr *)&server, sizeof(server));
    listen(s, 3);

    printf("Server waiting...\n");
    c = accept(s, NULL, NULL);

    recv(c, &N, sizeof(N), 0);
    recv(c, a, N * N * sizeof(int), 0);

    int upper = 1, lower = 1, diagonal = 1;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            if (i > j && a[i][j] != 0) upper = 0;
            if (i < j && a[i][j] != 0) lower = 0;
            if (i != j && a[i][j] != 0) diagonal = 0;
        }

    if (diagonal) strcpy(result, "Diagonal Matrix");
    else if (upper) strcpy(result, "Upper Triangular Matrix");
    else if (lower) strcpy(result, "Lower Triangular Matrix");
    else strcpy(result, "Not a special matrix");

    send(c, result, strlen(result) + 1, 0);

    close(c);
    close(s);
    return 0;
}