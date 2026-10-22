#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <netinet/ip.h>
#define BUFFER_SIZE 65536
int main(void) {
    int raw_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    unsigned char *buffer = malloc(BUFFER_SIZE);
    if (raw_socket < 0 || !buffer) { perror("Socket/Memory Error"); return 1; }
    int packet_count = 0;
    printf("Packet Capture Started... (Ctrl+C to stop)\n");
    while (1) {
        int packet_size = recvfrom(raw_socket, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (packet_size < 14 + (int)sizeof(struct iphdr)) { if (packet_size < 0) { perror("Receive Error"); break; } continue; }
        struct ethhdr *ethernet_header = (struct ethhdr *)buffer;
        if (ntohs(ethernet_header->h_proto) != ETH_P_IP) continue;
        struct iphdr *ip_header = (struct iphdr *)(buffer + sizeof(struct ethhdr));
        struct in_addr source_address = {.s_addr = ip_header->saddr};
        struct in_addr destination_address = {.s_addr = ip_header->daddr};
        printf("\nPacket No: %d\nPacket Size: %d bytes\nSource IP: %s\nDestination IP: %s\n--------------------------\n",
               ++packet_count, packet_size, inet_ntoa(source_address), inet_ntoa(destination_address));
    }
    close(raw_socket); free(buffer); return 0;
}