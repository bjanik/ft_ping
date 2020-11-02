/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 12:09:19 by bjanik            #+#    #+#             */
/*   Updated: 2020/07/27 12:09:21 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <sys/time.h>

int ft_ping_usage(void)
{
	dprintf(STDERR_FILENO, "Usage: ft_ping [-Ddqv] [-c count] [-s packetsize] destination\n");
	return (1);
}

static int	init_socket(int *opt)
{
	if (g_ping.ipv == IPV4)
		g_ping.socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (g_ping.socket_fd < 0)
	{
		perror("socket");
		return (1);
	}
	if (g_ping.opts.opt & PING_SOCKET_DEBUG)
	{
		if (setsockopt(g_ping.socket_fd, SOL_SOCKET, SO_DEBUG, opt, sizeof(*opt)))
		{
			perror("setsockopt");
			return (1);
		}
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
	ft_memset(&g_ping.opts, 0x0, sizeof(struct s_opts));
	g_ping.timing = 0;
	g_ping.transmitted = 0;
	g_ping.ttl = DEFAULT_TTL;
	g_ping.rtt_sum = 0;
	g_ping.rtt_max = 0;
	g_ping.rtt_min = INT_MAX;
	gettimeofday(&g_ping.start, NULL);
}

static int	init_ipv4(void)
{
	struct addrinfo		hints;
	struct addrinfo		*res;
	// struct hostent		*host;

	res = NULL;
	ft_memset(&hints, 0x0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_CANONNAME;
	if (getaddrinfo(g_ping.dest_name, NULL, &hints, &res))
	{
		perror("ft_ping");
		return (1);
	}
	g_ping.dest.sin_family = AF_INET;
	g_ping.dest.sin_addr.s_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr;
	// host = gethostbyname(g_ping.dest_name);
	// printf("%s\n", host->h_name);
	g_ping.canonname = ft_strdup(res->ai_canonname);
	if (res->ai_canonname == NULL)
		return (1);
	inet_ntop(g_ping.dest.sin_family, &g_ping.dest.sin_addr, g_ping.ipstr, 64);
	freeaddrinfo(res);
	return (0);
}

int			main(int argc, char **argv)
{
	int					opt;
	int					ret;

	ret = 0;
	if (argc == 1)
		return (ft_ping_usage());
	init_ping();
	if (parse_options(argc, argv))
		return (1);
	if (init_ipv4())
		return (1);
	init_socket(&opt);
	signal(SIGALRM, sig_alarm);
	signal(SIGINT, sig_final);
	printf("PING %s (%s) %d(%ld) bytes of data.\n", g_ping.dest_name,
			g_ping.ipstr, g_ping.datalen,
			g_ping.datalen + sizeof(struct iphdr) + sizeof(struct icmphdr));
	sig_alarm(0);
	recv_pong();
	return (ret);
}
