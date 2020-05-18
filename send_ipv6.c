#include "ft_ping.h"

int 	send_ipv6(void)
{
	void				*pkt;
	unsigned char 		*p;
	struct icmp6hdr 	*icmp6;
	struct timeval 		*tv;
	int 				pkt_size;
	int 				ret;

	// Allocated space for packet
	pkt_size = g_ping.datalen + ICMP6_HDR_SIZE;
	if ((pkt = malloc(pkt_size)) == NULL)
		return (1);
	icmp6 = (struct icmp6hdr*)pkt;
	icmp6->icmp6_type = ICMPV6_ECHO_REQUEST;
	icmp6->icmp6_code = 0;
	icmp6->icmp6_identifier = g_ping.id;
	icmp6->icmp6_cksum = 0;
	icmp6->icmp6_sequence = ++g_ping.transmitted;
	if (g_ping.datalen >= TIMEVAL_SIZE)
	{
		g_ping.timing = 1;
		tv = (struct timeval*)(icmp6 + 1);
		gettimeofday(tv, NULL);
	}
	p = (unsigned char*)(tv + 1);
	for (int i = TIMEVAL_SIZE; i < g_ping.datalen; i++)
		*p++ = i;
	icmp6->icmp6_cksum = in_cksum((void*)icmp6, pkt_size);
	ret = sendto(g_ping.socket_fd, 
				 pkt,
				 pkt_size,
				 0,
				 (struct sockaddr *)&g_ping.dest6,
				 sizeof(g_ping.dest6));
	if (ret < 0)
	{
		perror("sendto");
		free(pkt);
		return (ret);
	}
	free(pkt);
	return (ret);
}