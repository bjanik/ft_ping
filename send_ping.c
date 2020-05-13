#include "ft_ping.h"

int 	send_ping(void)
{
	void			*pkt;
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
	icmp->checksum = in_cksum((void*)icmp, g_ping.datalen);
	icmp->un.echo.id = g_ping.id;
	icmp->un.echo.sequence = ++g_ping.transmitted;
	if (g_ping.datalen >= 16)
	{
		g_ping.timing = 1;
		tv = (struct timeval*)(icmp + 1);
		gettimeofday(tv, NULL);
	}
	ret = sendto(g_ping.socket_fd, 
				 pkt,
				 g_ping.datalen,
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