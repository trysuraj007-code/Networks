#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 9000
#define MAX 10

int main() {
    int s, c, clients[MAX] = {0};
    struct sockaddr_in addr;
    fd_set fds;
    char buf[1024];

    s = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    listen(s, 5);

    printf("Chat Server Started...\n");

    while (1) {
        FD_ZERO(&fds);
        FD_SET(s, &fds);
        int max = s;

        for (int i = 0; i < MAX; i++) {
            if (clients[i] > 0) {
                FD_SET(clients[i], &fds);
                if (clients[i] > max) max = clients[i];
            }
        }

        select(max + 1, &fds, NULL, NULL, NULL);

        if (FD_ISSET(s, &fds)) {
            c = accept(s, NULL, NULL);
            for (int i = 0; i < MAX; i++)
                if (clients[i] == 0) {
                    clients[i] = c;
                    break;
                }
        }

        for (int i = 0; i < MAX; i++) {
            int sd = clients[i];

            if (sd > 0 && FD_ISSET(sd, &fds)) {
                int n = recv(sd, buf, sizeof(buf), 0);

                if (n <= 0) {
                    close(sd);
                    clients[i] = 0;
                } else {
                    for (int j = 0; j < MAX; j++)
                        if (clients[j] > 0 && clients[j] != sd)
                            send(clients[j], buf, n, 0);
                }
            }
        }
    }

    close(s);
    return 0;
}