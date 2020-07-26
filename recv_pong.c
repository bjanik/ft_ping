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


static const char *icmp_responses[] = {
	[ICMP_DEST_UNREACH] = "Host unreachable",
	[ICMP_SOURCE_QUENCH] = "Source Quench",
	//[ICMP_ECHO] = "Echo",	
	[ICMP_REDIRECT] = "Redirect",
	[ICMP_TIME_EXCEEDED] = "Time to live exceeded",
	[ICMP_PARAMETERPROB] = "Parameter Problem",
	[ICMP_TIMESTAMP] = "Timestamp",
	[ICMP_TIMESTAMPREPLY] = "Timestamp reply",
	[ICMP_INFO_REQUEST] = "Info Request",
	[ICMP_INFO_REPLY] = "Info Reply",
	[ICMP_ADDRESS] = "",
	[ICMP_ADDRESSREPLY] = "",

};

static int read_pong_ipv4(void *buf, int len)
{
	struct icmphdr		*icmp;
	struct iphdr 		*ip;
	struct timeval 		tv;
	struct timeval 		*time;
	float				rtt;

	g_ping.timing ?	gettimeofday(&tv, NULL) : 0;
	ip = (struct iphdr*)buf;
	icmp = (struct icmphdr*)(buf + sizeof(struct iphdr));
	g_ping.timing ?	time = (struct timeval*)(icmp + 1) : 0 ;
	if (icmp->type != ICMP_ECHOREPLY)
	{
		if (icmp->type == ICMP_ECHO)
			return 
		dprintf(STDERR_FILENO, "%s\n", icmp_responses[icmp->type]);
		return (1);
	}
	if (icmp->un.echo.id != g_ping.id)
	{
		dprintf(STDERR_FILENO, "ID not matching\n");
		return (1);
	}
	printf("%lu bytes from %s", g_ping.datalen + sizeof(struct icmphdr), g_ping.dest_name);
	strcmp(g_ping.ipstr, g_ping.dest_name) ? printf(" (%s)", g_ping.ipstr) : 0;
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
	int 				fromlen;
	int 				len;
	unsigned char		buf[1024];
	struct sockaddr_in	from;

	while (42)
	{
		fromlen = sizeof(struct sockaddr_in);
		len = recvfrom(g_ping.socket_fd, (void*)buf, g_ping.datalen + 100, 0, (struct sockaddr *) &from, &fromlen);
		read_pong_ipv4(buf, len);
	}
	return (0);
}