/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_ipv4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 12:09:50 by bjanik            #+#    #+#             */
/*   Updated: 2020/07/27 12:09:52 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void fill_iphdr(struct iphdr *ip)
{
	ip->version = 4;
	ip->ihl = IP_HDR_SIZE / 4;
	ip->tos = IP_TOS;
	ip->tot_len = g_ping.datalen + ICMP_HDR_SIZE;
	ip->id = 0;
	ip->frag_off = 0;
	ip->ttl = g_ping.ttl;
	ip->protocol = IPPROTO_ICMP;
	ip->check = 0;
	ip->saddr = INADDR_ANY;
	ip->daddr = g_ping.dest.sin_addr.s_addr;
}

static void	fill_pkt(void *pkt)
{
	unsigned char	*p;
	struct timeval	*tv;
	struct iphdr 	*ip = pkt;
	struct icmphdr 	*icmp;

	fill_iphdr(ip);
	icmp = (struct icmphdr*)(ip + 1);
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->un.echo.id = g_ping.id;
	icmp->checksum = 0;
	icmp->un.echo.sequence = ++g_ping.transmitted;
	if (g_ping.datalen >= TIMEVAL_SIZE)
	{
		tv = (struct timeval*)(icmp + 1);
		g_ping.timing = 1;
		gettimeofday(tv, NULL);
	}
	p = (unsigned char*)icmp + ICMP_HDR_SIZE + sizeof(struct timeval);
	for (int i = 0; i < g_ping.datalen; i++)
		*p++ = i++;
	icmp->checksum = in_cksum((void*)icmp, g_ping.datalen +
								sizeof(struct icmphdr));
}

int			send_ipv4(void)
{
	void			*pkt;
	int				pkt_size = g_ping.datalen + ICMP_HDR_SIZE + IP_HDR_SIZE;
	int				ret;
	
	if ((pkt = malloc(pkt_size)) == NULL)
		return (1);
	fill_pkt(pkt);
	ret = sendto(g_ping.socket_fd, pkt,
				pkt_size, 0,
				(struct sockaddr*)&g_ping.dest, sizeof(g_ping.dest));
	if (ret < 0)
		perror("sendto");
	free(pkt);
	return ret;
}
