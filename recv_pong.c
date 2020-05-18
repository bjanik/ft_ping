#include "ft_ping.h"

// Substracting tv - time and store result in tv
void ft_tv_sub(struct timeval *tv, struct timeval *time)
{
	tv->tv_usec -= time->tv_usec;
	if (tv->tv_usec < 0)
	{
		tv->tv_usec += 1000000;
		tv->tv_sec--;
	}
	tv->tv_sec -= time->tv_sec;
}


static int read_pong_ipv6(void *buf, int len)
{
	struct icmp6hdr		*icmp6;
	struct ipv6hdr 		*ipv6;
	struct timeval 		tv, *time;
	float				rtt;

	if (g_ping.timing)
		gettimeofday(&tv, NULL);
	ipv6 = (struct ipv6hdr*)buf;
	icmp6 = (struct icmp6hdr*)(ipv6 + 1);
	if (g_ping.timing)
		time = (struct timeval*)(icmp6 + 1);
	if (icmp6->icmp6_type != ICMPV6_ECHO_REPLY)
		return (1);
	if (icmp6->icmp6_identifier != g_ping.id)
	{
		dprintf(STDERR_FILENO, "ID not matching\n");
		return (1);
	}
	printf("%lu bytes from %s", g_ping.datalen + ICMP6_HDR_SIZE, g_ping.dest_name);
	if (strcmp(g_ping.ipstr, g_ping.dest_name))
		printf(" (%s)", g_ping.ipstr);
	printf(": icmp_seq=%d ttl=%hhu", icmp6->icmp6_sequence, ipv6->hop_limit);
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
	g_ping.received++;
	printf("\n");
	return (0);
}

static int read_pong_ipv4(void *buf, int len)
{
	struct icmphdr		*icmp;
	struct iphdr 		*ip;
	struct timeval 		tv, *time;
	float				rtt;

	if (g_ping.timing)
		gettimeofday(&tv, NULL);
	ip = (struct iphdr*)buf;
	icmp = (struct icmphdr*)(buf + sizeof(struct iphdr));
	if (g_ping.timing)
		time = (struct timeval*)(icmp + 1);
	if (icmp->type != ICMP_ECHOREPLY)
		return (1);
	if (icmp->un.echo.id != g_ping.id)
	{
		dprintf(STDERR_FILENO, "ID not matching\n");
		return (1);
	}
	printf("%lu bytes from %s", g_ping.datalen + ICMP_HDR_SIZE, g_ping.dest_name);
	if (strcmp(g_ping.ipstr, g_ping.dest_name))
		printf(" (%s)", g_ping.ipstr);
	printf(": icmp_seq=%d ttl=%hhu", icmp->un.echo.sequence, ip->ttl);
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
	g_ping.received++;
	printf("\n");
	return (0);
}


int	recv_pong(void)
{
	int 				fromlen, len;
	unsigned char		buf[1024];
	struct sockaddr_in	from;

	while (42)
	{
		fromlen = sizeof(struct sockaddr_in);
		len = recvfrom(g_ping.socket_fd, (void*)buf, g_ping.datalen + 100, 0, (struct sockaddr *) &from, &fromlen);
		if (g_ping.ipv == IPV4)
			read_pong_ipv4(buf, len);
		else
			read_pong_ipv6(buf, len);
	}
	return (0);
}