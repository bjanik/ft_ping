/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checksum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 12:09:33 by bjanik            #+#    #+#             */
/*   Updated: 2020/07/27 12:09:35 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

unsigned short	in_cksum(void *ptr, int len)
{
	unsigned short	*p;
	unsigned int	checksum;
	unsigned short	oddbyte;
	unsigned short	res;

	checksum = 0;
	p = ptr;
	while (len > 1)
	{
		checksum += *p++;
		len -= sizeof(unsigned short);
	}
	if (len == 1)
	{
		oddbyte = 0;
		*((unsigned short *)&oddbyte) = *(unsigned char *)ptr;
	}
	checksum = (checksum >> 16) + (checksum & 0xFFFF);
	checksum += (checksum >> 16);
	res = ~checksum;
	return (res);
}
