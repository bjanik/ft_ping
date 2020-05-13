#include "ft_ping.h"
#include <sys/time.h>


int		init_socket(int *opt)
{
	g_ping.socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g_ping.socket_fd < 0)
	{
		perror("socket");
		return (1);
	}
	// if (setsockopt(g_ping.socket_fd, SOL_SOCKET, 0, opt, sizeof(*opt)))
	// {
	// 	perror("setsockopt");
	// 	return (1);	
	// }
	return (0);
}

void	init_ping(void)
{
	g_ping.id = getpid();
	g_ping.dest_name = NULL;
	g_ping.datalen = DEFAULT_DATALEN;
	g_ping.ipstr = (char*)malloc(64);
	if (g_ping.ipstr == NULL)
		exit(1);
	memset(&g_ping.opts, 0x0, sizeof(struct s_opts));
	g_ping.timing = 0;
	g_ping.transmitted = 0;
	g_ping.rtt_sum = 0;
	g_ping.rtt_max = 0;
	g_ping.rtt_min = INT_MAX;
}

int main(int argc, char **argv)
{
	struct addrinfo 	hints;
	struct addrinfo 	*res = NULL;
	int 				opt, ret;
	struct msghdr 		msg;
	unsigned char		buf[1024];
	struct sockaddr_in 	from;
	int 				fromlen;

	if (argc == 1)
	{
		dprintf(2, "Usage\n");
		return (1);
	}
	init_ping();
	parse_options(argc, argv);
	memset(&hints, 0x0, sizeof(struct addrinfo));
	memset(buf, 0x0, 1024);
	hints.ai_family = AF_INET;
	if (getaddrinfo(g_ping.dest_name, NULL, &hints, &res))
		return (1);
	g_ping.dest.sin_family = AF_INET;
	g_ping.dest.sin_addr.s_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
	inet_ntop(g_ping.dest.sin_family, &g_ping.dest.sin_addr, g_ping.ipstr, 64);
	freeaddrinfo(res);
	init_socket(&opt);
	signal(SIGALRM, sig_alarm);
	printf("PING %s (%s) %d(%ld) bytes of data.\n", g_ping.dest_name, g_ping.ipstr, g_ping.datalen, g_ping.datalen + IP_HDR_SIZE + ICMP_HDR_SIZE);
	sig_alarm(0);
	recv_pong();
	return (ret);
}