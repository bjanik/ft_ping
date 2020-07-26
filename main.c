#include "ft_ping.h"
#include <sys/time.h>


static int		init_socket(int *opt)
{
	if (g_ping.ipv == IPV4)
		g_ping.socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g_ping.socket_fd < 0)
	{
		perror("socket");
		return (1);
	}
	if (setsockopt(g_ping.socket_fd, SOL_SOCKET, 0, opt, sizeof(*opt)))
	{
		perror("setsockopt");
	 	return (1);	
	}
	return (0);
}

static void	init_ping(void)
{
	g_ping.id = getpid();
	g_ping.dest_name = NULL;
	g_ping.datalen = DEFAULT_DATALEN;
	g_ping.ipstr = (char*)malloc(64);
	if (g_ping.ipstr == NULL)
		exit(1);
	g_ping.ipv = IPV4;
	memset(&g_ping.opts, 0x0, sizeof(struct s_opts));
	g_ping.timing = 0;
	g_ping.transmitted = 0;
	g_ping.rtt_sum = 0;
	g_ping.rtt_max = 0;
	g_ping.rtt_min = INT_MAX;
	gettimeofday(&g_ping.start, NULL);
}

static int	init_ipv4(void)
{
	struct addrinfo 	hints;
	struct addrinfo 	*res = NULL;

	memset(&hints, 0x0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_CANONNAME;
	if (getaddrinfo(g_ping.dest_name, NULL, &hints, &res))
		return (1);
	g_ping.dest.sin_family = AF_INET;
	g_ping.dest.sin_addr.s_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
	g_ping.canonname = strdup(res->ai_canonname);
	inet_ntop(g_ping.dest.sin_family, &g_ping.dest.sin_addr, g_ping.ipstr, 64);
	freeaddrinfo(res);
	return (0);
}

//static int	init_ipv6(void)
//{
//	struct addrinfo 	hints;
//	struct addrinfo 	*res = NULL;
//
//	memset(&hints, 0x0, sizeof(struct addrinfo));
//	hints.ai_family = AF_INET6;
//	if (getaddrinfo(g_ping.dest_name, NULL, &hints, &res))
//		return (1);
//	g_ping.dest6.sin6_family = AF_INET6;
//	memcpy(g_ping.dest6.sin6_addr.s6_addr,
//		   ((struct sockaddr_in6*)res->ai_addr)->sin6_addr.s6_addr, 16);
//	// g_ping.dest6.sin6_addr.s6_addr = ((struct sockaddr_in6*)res->ai_addr)->sin6_addr.s6_addr;
//	inet_ntop(g_ping.dest6.sin6_family, &g_ping.dest6.sin6_addr, g_ping.ipstr, 64);
//	freeaddrinfo(res);
//	return (0);
//}

int main(int argc, char **argv)
{
	int 				opt;
	int 				ret;

	if (argc == 1)
	{
		dprintf(2, "Usage\n");
		return (1);
	}
	init_ping();
	if (parse_options(argc, argv))
		return (1);
	init_ipv4();
	init_socket(&opt);
	signal(SIGALRM, sig_alarm);
	signal(SIGINT, sig_final);
	printf("PING %s (%s) %d(%ld) bytes of data.\n", g_ping.dest_name, g_ping.ipstr, g_ping.datalen, g_ping.datalen + IP_HDR_SIZE + ICMP_HDR_SIZE);
	sig_alarm(0);
	recv_pong();
	return (ret);
}