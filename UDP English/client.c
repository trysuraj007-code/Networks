#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

int main() {
    int s;
    char buf[1024];
    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    s = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter sentence: ");
    fgets(buf, sizeof(buf), stdin);

    sendto(s, buf, strlen(buf)+1, 0,
           (struct sockaddr *)&server, len);

    recvfrom(s, buf, sizeof(buf)-1, 0,
             (struct sockaddr *)&server, &len);

    printf("Translated: %s\n", buf);

    close(s);
    return 0;
}