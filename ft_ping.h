#ifndef FT_PING
#define FT_PING
// #include <sys/types.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
// #include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <unistd.h>

#define ICMP_HDR_SIZE 8
#define TIMEVAL_SIZE 16
#define DEFAULT_DATALEN 56


struct s_ping
{
	struct sockaddr_in	dest;
	int 				socket_fd;
	int					transmitted;
	int					received;
	pid_t 				ident;
	int 				timing;
	int 				datalen;
};

#endif