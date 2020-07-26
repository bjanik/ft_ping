#ifndef FT_PING
#define FT_PING
#include <arpa/inet.h>
#include <linux/sockios.h>
#include <linux/icmp.h>
#include <linux/ip.h>
#include <limits.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define ICMP_HDR_SIZE sizeof(struct icmphdr)
#define IP_HDR_SIZE sizeof(struct iphdr)
#define TIMEVAL_SIZE 16
#define DEFAULT_DATALEN 56

#define IPV4 0
#define PING_QUIET 1
#define PING_SOCKET_DEBUG 2
#define PING_VERBOSE 4
#define PING_CAP_D 8

struct s_opts
{
	long 	count;
	int 	opt;
};

struct s_ping
{
	struct sockaddr_in	dest;
	char				*dest_name;
	char				*canonname;
	char				*ipstr;
	int 				ipv;
	int 				socket_fd;
	int					transmitted;
	int					received;
	pid_t 				id;
	int 				timing;
	int 				datalen;
	float 				rtt_sum; // in ms
	float				rtt_max; // in ms
	float 				rtt_min; // in ms
	struct timeval 		start;
	struct s_opts		opts;
};

int 					send_ipv4(void);
int						recv_pong(void);

unsigned short 			in_cksum(void *ptr, int len);
int 					parse_options(int argc, char **argv);
void 					ft_tv_sub(struct timeval *tv, struct timeval *time);

void					sig_alarm(int x);
void 					sig_final(int x);


struct s_ping 			g_ping;

#endif