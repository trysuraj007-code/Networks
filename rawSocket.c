#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>

int main() {
    int s, n;
    unsigned char buf[65536];

    s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (s < 0) {
        perror("Socket");
        return 1;
    }

    printf("Packet Capture Started...\n");

    while (1) {
        n = recvfrom(s, buf, sizeof(buf), 0, NULL, NULL);
        if (n < 0) continue;

        struct ethhdr *eth = (struct ethhdr *)buf;
        printf("\nPacket Size: %d\n", n);
        printf("Source MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
               eth->h_source[0], eth->h_source[1],
               eth->h_source[2], eth->h_source[3],
               eth->h_source[4], eth->h_source[5]);

        if (ntohs(eth->h_proto) == ETH_P_IP) {
            struct iphdr *ip = (struct iphdr *)(buf + sizeof(struct ethhdr));

            printf("Source IP: %s\n", inet_ntoa(*(struct in_addr *)&ip->saddr));
            printf("Destination IP: %s\n", inet_ntoa(*(struct in_addr *)&ip->daddr));

            if (ip->protocol == IPPROTO_TCP)
                printf("Protocol: TCP\n");
            else if (ip->protocol == IPPROTO_UDP)
                printf("Protocol: UDP\n");
            else if (ip->protocol == IPPROTO_ICMP)
                printf("Protocol: ICMP\n");
        }
    }

    close(s);
    return 0;
}