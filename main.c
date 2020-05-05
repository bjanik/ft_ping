#include "ft_ping.h"
#include <sys/time.h>
#include <stdio.h>

struct s_ping 	g_ping;

int		init_socket(int *opt)
{
	g_ping.socket_fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g_ping.socket_fd < 0)
	{
		perror("socket");
		return (1);
	}
	if (setsockopt(g_ping.socket_fd, SOL_SOCKET, IP_HDRINCL, opt, sizeof(*opt)))
	{
		perror("setsockopt");
		return (1);	
	}
	return (0);
}

// int 	send_ping()
// {
// 	void			*pkt;
// 	struct icmp 	*icmp;

// 	pkt = malloc(g_ping.datalen + ICMP_HDR_SIZE);
// 	icmp = pkt;
// 	icmp->type = ICMP_ECHO;
// 	icmp->code = 0;
// 	icmp->ident = g_ping.ident;
// 	icmp->
// 	if (pkt == NULL)
// 		return (1);
// 	return (0);
// }

void 	handle_alarm()
{
	printf("ALARM\n");
}

int main(int argc, char **argv)
{
	struct hostent		*host;
	struct addrinfo 	hints;
	struct addrinfo 	*res = NULL, *rp = NULL;
	int 				opt;

	if (argc == 1)
	{
		dprintf(2, "Usage\n");
		return (1);
	}
	// NEED PARSING OPTIONS

	memset(&hints, 0x0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	if (getaddrinfo(argv[1], NULL, &hints, &res))
		return (1);
	g_ping.dest.sin_addr.s_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
	freeaddrinfo(res);
	g_ping.ident = getpid();
	printf("%u\n", g_ping.dest.sin_addr.s_addr);
	init_socket(&opt);
	signal(SIGALRM, handle_alarm);
	while (1)
	{
		alarm(1);
	}
	// for (rp = res; rp != NULL; rp = rp->ai_next)
	// {
	// 	printf("toto\n");
	// }
	return (0);
}