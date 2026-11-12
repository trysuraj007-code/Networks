#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000

int main() {
    int s;
    char file[100], buf[2048];
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    s = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(s, (struct sockaddr *)&server, sizeof(server));

    printf("UDP File Server Started...\n");

    while (1) {
        memset(file, 0, sizeof(file));
        recvfrom(s, file, sizeof(file)-1, 0,
                 (struct sockaddr *)&client, &len);

        if (fork() == 0) {
            FILE *fp = fopen(file, "r");
            memset(buf, 0, sizeof(buf));

            if (fp) {
                fread(buf, 1, sizeof(buf)-1, fp);
                fclose(fp);
            } else {
                strcpy(buf, "File Not Found");
            }

            sendto(s, buf, strlen(buf)+1, 0,
                   (struct sockaddr *)&client, len);
            _exit(0);
        }
    }

    close(s);
    return 0;
}