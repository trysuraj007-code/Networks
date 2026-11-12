#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000

int main() {
    int s;
    struct sockaddr_in server;
    char name[30], msg[1024], buf[1024];

    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(s, (struct sockaddr *)&server, sizeof(server));

    printf("Enter name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    send(s, name, sizeof(name), 0);

    if (fork() == 0) {
        while (fgets(msg, sizeof(msg), stdin))
            send(s, msg, strlen(msg), 0);
    } else {
        int n;
        while ((n = recv(s, buf, sizeof(buf)-1, 0)) > 0) {
            buf[n] = '\0';
            printf("%s", buf);
        }
    }

    close(s);
    return 0;
}