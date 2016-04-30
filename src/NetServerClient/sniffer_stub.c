/*
 * sniffer_stub.c
 *
 *  Created on: 2016年4月29日
 *      Author: wang
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <netinet/if_ether.h>

//============================
#define MAX_BUF     2048        /* Maximum bytes fetched by a single read() */

int set_nic_promisc(int sockfd, const char *nic_name)
{
    struct ifreq ethreq;
    strncpy(ethreq.ifr_name, nic_name, IFNAMSIZ);
    if( ioctl(sockfd, SIOCGIFFLAGS, &ethreq) == -1 )
    {
        perror ("Error: Could not retrieve the flags from the device.\n");
        exit (1);
    }
    ethreq.ifr_flags |= IFF_PROMISC;
    if( ioctl(sockfd, SIOCSIFFLAGS, &ethreq) == -1 )
    {
        perror ("Error: Could not set flag IFF_PROMISC");
        exit (1);
    }
    return 0;
}

int raw_init (const char *device)
{
    struct ifreq ifr;
    int raw_socket;

    memset (&ifr, 0, sizeof (struct ifreq));

    /* Open A Raw Socket */
    if ((raw_socket = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 1)
    {
        printf ("ERROR: Could not open socket, Got #?\n");
        exit (1);
    }

    /* Set the device to use */
    strcpy (ifr.ifr_name, device);

    /* Get the current flags that the device might have */
    if (ioctl (raw_socket, SIOCGIFFLAGS, &ifr) == -1)
    {
        perror ("Error: Could not retrive the flags from the device.\n");
        exit (1);
    }

    /* Set the old flags plus the IFF_PROMISC flag */
    ifr.ifr_flags |= IFF_PROMISC;
    if (ioctl (raw_socket, SIOCSIFFLAGS, &ifr) == -1)
    {
        perror ("Error: Could not set flag IFF_PROMISC");
        exit (1);
    }
    printf ("Entering promiscuous mode\n");

    /* Configure the device */

    if (ioctl (raw_socket, SIOCGIFINDEX, &ifr) < 0)
    {
        perror ("Error: Error getting the device index.\n");
        exit (1);
    }

    return raw_socket;
}

//int main(int argc, char **argv) {
int main_sniffer_stub(int argc, char **argv) {
    int sock, n;
    char buffer[2048];
    struct ethhdr *eth;
    struct iphdr *iph;

    if (0 > (sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP)))) {
        perror("socket");
        exit(1);
    }

    //set_nic_promisc(sock,"eth0");

    //sock = raw_init("eth0");

    while (1) {
        printf("=====================================\n");
        //注意：在这之前我没有调用bind函数，原因是什么呢？
        n = recvfrom(sock, buffer, 2048, 0, NULL, NULL);
        printf("%d bytes read\n", n);

        //接收到的数据帧头6字节是目的MAC地址，紧接着6字节是源MAC地址。
        eth = (struct ethhdr*) buffer;
        printf("Dest MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
        printf("Source MAC addr:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);

        iph = (struct iphdr*) (buffer + sizeof(struct ethhdr));
        //我们只对IPV4且没有选项字段的IPv4报文感兴趣
        if (iph->version == 4 && iph->ihl == 5) {
            struct in_addr src_addr, dst_addr;
            src_addr.s_addr = (in_addr_t) (iph->saddr);
            dst_addr.s_addr = (in_addr_t) (iph->saddr);

            printf("Source      host:%s\n", inet_ntoa(src_addr));
            printf("Destination host:%s\n", inet_ntoa(dst_addr));
        }
    }
}

