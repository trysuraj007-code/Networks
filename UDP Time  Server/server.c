#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 9000

int main() {
    int s;
    char buf[100];
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    s = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(s, (struct sockaddr *)&server, sizeof(server));

    printf("UDP Time Server Started...\n");

    while (1) {
        recvfrom(s, buf, sizeof(buf), 0,
                 (struct sockaddr *)&client, &len);

        time_t t = time(NULL);
        char *msg = ctime(&t);

        sendto(s, msg, strlen(msg)+1, 0,
               (struct sockaddr *)&client, len);

        printf("Time request served.\n");
    }

    close(s);
    return 0;
}