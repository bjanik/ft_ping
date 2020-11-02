/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_final.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 12:10:03 by bjanik            #+#    #+#             */
/*   Updated: 2020/07/27 12:10:04 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	sig_final(int x)
{
	float			packet_loss;
	struct timeval	end;

	(void)x;
	packet_loss = (g_ping.transmitted - g_ping.received) / (float)g_ping.transmitted;
	gettimeofday(&end, NULL);
	ft_tv_sub(&end, &g_ping.start);
	printf("\n--- %s ping statistics ---\n", g_ping.dest_name);
	printf("%d packets transmitted, ", g_ping.transmitted);
	printf("%d packets received, ", g_ping.received);
	printf("%d%% packet loss, ", (int)(packet_loss * 100));
	printf("time %ldms\n", end.tv_sec * 1000 + end.tv_usec / 1000);
	if (g_ping.timing)
		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
				g_ping.rtt_min,
				g_ping.rtt_sum / g_ping.transmitted, g_ping.rtt_max, 0.0);
	exit(0);
}
