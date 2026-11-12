#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

int main() {
    int s;
    char buf[1024];
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    char *abbr[] = {"tbh","ig","lol","idk","omg"};
    char *full[] = {"to be honest","I guess","laughing out loud",
                    "I don't know","oh my God"};

    s = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(s, (struct sockaddr *)&server, sizeof(server));

    recvfrom(s, buf, sizeof(buf)-1, 0,
             (struct sockaddr *)&client, &len);
    buf[sizeof(buf)-1] = '\0';

    char result[1024] = "";
    char *word = strtok(buf, " ");

    while (word) {
        int found = 0;
        for (int i = 0; i < 5; i++)
            if (!strcmp(word, abbr[i])) {
                strcat(result, full[i]);
                found = 1;
                break;
            }
        if (!found) strcat(result, word);
        strcat(result, " ");
        word = strtok(NULL, " ");
    }

    sendto(s, result, strlen(result)+1, 0,
           (struct sockaddr *)&client, len);

    close(s);
    return 0;
}