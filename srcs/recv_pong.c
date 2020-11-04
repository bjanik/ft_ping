/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recv_pong.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 12:10:38 by bjanik            #+#    #+#             */
/*   Updated: 2020/07/27 12:10:39 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void				ft_tv_sub(struct timeval *tv, struct timeval *time)
{
	tv->tv_usec -= time->tv_usec;
	if (tv->tv_usec < 0)
	{
		tv->tv_usec += 1000000;
		tv->tv_sec--;
	}
	tv->tv_sec -= time->tv_sec;
}


static void			display_reply_update_timing(struct icmphdr *icmp,
												struct timeval tv,
												struct timeval *time,
												struct ip *ip)
{
	float rtt = 0;

	printf("%lu bytes from %s", g_ping.datalen + sizeof(struct icmphdr),
			g_ping.h_name);
	if (ft_strcmp(g_ping.ipstr, g_ping.dest_name))
		printf(" (%s)", g_ping.ipstr);
	printf(": icmp_seq=%d ttl=%hhu", icmp->un.echo.sequence, ip->ip_ttl);
	if (g_ping.timing)
	{
		ft_tv_sub(&tv, time);
		rtt = tv.tv_sec * 1000 + (float)tv.tv_usec / 1000;
		if (rtt < g_ping.rtt_min)
			g_ping.rtt_min = rtt;
		if (rtt > g_ping.rtt_max)
			g_ping.rtt_max = rtt;
		g_ping.rtt_sum += rtt;
		printf(" time=%.3f ms", rtt);
	}
	printf("\n");
}

static int handle_reply_error(struct ip *ip, struct icmphdr *icmp)
{
	char 				*sender_ip_str;
	struct in_addr		saddr = {.s_addr = ip->ip_src.s_addr};
	struct hostent 		*host;

	static const char	*g_icmp_responses[] = {
		[ICMP_DEST_UNREACH] = "Host unreachable",
		[ICMP_SOURCE_QUENCH] = "Source Quench",
		[ICMP_ECHO] = "Echo",
		[ICMP_REDIRECT] = "Redirect",
		[ICMP_TIME_EXCEEDED] = "Time to live exceeded",
		[ICMP_PARAMETERPROB] = "Parameter Problem",
		[ICMP_TIMESTAMP] = "Timestamp",
		[ICMP_TIMESTAMPREPLY] = "Timestamp reply",
		[ICMP_INFO_REQUEST] = "Info Request",
		[ICMP_INFO_REPLY] = "Info Reply",
		[ICMP_ADDRESS] = "Address",
		[ICMP_ADDRESSREPLY] = "Address Reply",
	};
	sender_ip_str = inet_ntoa(saddr);
	host = gethostbyaddr((char*)&saddr.s_addr, sizeof(int), AF_INET);
	if (host)
		dprintf(STDERR_FILENO, "From %s, (%s) icmp_seq=%d %s\n", host->h_name, sender_ip_str, g_ping.transmitted, g_icmp_responses[icmp->type]);
	else
		dprintf(STDERR_FILENO, "From %s, icmp_seq=%d %s\n", sender_ip_str, g_ping.transmitted, g_icmp_responses[icmp->type]);
	return (1);
}
static int			read_pong_ipv4(void *pkt)
{
	struct ip			*ip = pkt;
	struct icmphdr		*icmp = pkt + IP_HDR_SIZE;
	struct timeval		tv;
	struct timeval		*time = NULL;

	g_ping.timing ? gettimeofday(&tv, NULL) : 0;
	g_ping.timing ? time = (struct timeval*)(icmp + 1) : 0;
	if (icmp->type != ICMP_ECHOREPLY)
	{
		if (icmp->type == ICMP_ECHO)
			return (0);
		g_ping.errors++;
		return handle_reply_error(ip, icmp);
	}
	if (icmp->un.echo.id != g_ping.id)
		return 1;
	display_reply_update_timing(icmp, tv, time, ip);
	g_ping.received++;
	return 0;
}

static int recv_pkt(void *pkt, int pkt_size)
{
	socklen_t			fromlen;
	struct sockaddr_in	from;
	int				len;

	len = recvfrom(g_ping.socket_fd,
					pkt,
					pkt_size,
					0,
					(struct sockaddr *)&from, &fromlen);
	if (len > 0)
		read_pong_ipv4(pkt);
	return 0;
}

int					recv_pong(void)
{
	unsigned char		buf[2048];

	while (42)
		recv_pkt((void*)buf, g_ping.datalen + ICMP_HDR_SIZE + IP_HDR_SIZE);
	return 0;
}
