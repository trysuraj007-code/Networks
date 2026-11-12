#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000

int main() {
    int s;
    char buf[100] = "TIME";
    struct sockaddr_in server;

    s = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    sendto(s, buf, strlen(buf)+1, 0,
           (struct sockaddr *)&server, sizeof(server));

    int n = recvfrom(s, buf, sizeof(buf)-1, 0, NULL, NULL);
    if (n > 0) {
        buf[n] = '\0';
        printf("Current Server Time:\n%s", buf);
    }

    close(s);
    return 0;
}