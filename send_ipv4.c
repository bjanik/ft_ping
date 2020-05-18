#include "ft_ping.h"

int 	send_ipv4(void)
{
	void			*pkt;
	unsigned char 	*p;
	struct icmphdr 	*icmp;
	struct timeval 	*tv;
	int 			pkt_size;
	int 			ret;

	// Allocated space for packet
	pkt_size = g_ping.datalen + ICMP_HDR_SIZE;
	if ((pkt = malloc(pkt_size)) == NULL)
		return (1);
	icmp = (struct icmphdr*)pkt;
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->un.echo.id = g_ping.id;
	icmp->checksum = 0;
	icmp->un.echo.sequence = ++g_ping.transmitted;
	if (g_ping.datalen >= TIMEVAL_SIZE)
	{
		g_ping.timing = 1;
		tv = (struct timeval*)(icmp + 1);
		gettimeofday(tv, NULL);
	}
	p = (unsigned char*)(tv + 1);
	for (int i = TIMEVAL_SIZE; i < g_ping.datalen; i++)
		*p++ = i;
	icmp->checksum = in_cksum((void*)icmp, pkt_size);
	ret = sendto(g_ping.socket_fd, 
				 pkt,
				 pkt_size,
				 0,
				 (struct sockaddr*)&g_ping.dest,
				 sizeof(g_ping.dest));
	if (ret < 0)
	{
		perror("sendto");
		free(pkt);
		return (ret);
	}
	free(pkt);
	return (ret);
}