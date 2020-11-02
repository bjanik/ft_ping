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

static void	fill_pkt(struct icmphdr *icmp)
{
	unsigned char	*p;
	struct timeval	*tv;
	int				i;

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
	i = 0;
	while (i < g_ping.datalen)
		*p++ = i++;
	icmp->checksum = in_cksum((void*)icmp, g_ping.datalen +
								sizeof(struct icmphdr));
}

int			send_ipv4(void)
{
	void			*pkt;
	int				pkt_size = g_ping.datalen + ICMP_HDR_SIZE;
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
	return (ret);
}
