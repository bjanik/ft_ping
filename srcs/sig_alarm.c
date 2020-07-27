/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_alarm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 12:10:11 by bjanik            #+#    #+#             */
/*   Updated: 2020/07/27 12:10:19 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	sig_alarm(int x)
{
	(void)x;
	send_ipv4();
	if (g_ping.opts.count == 0 || g_ping.transmitted < g_ping.opts.count)
		alarm(1);
	else
	{
		signal(SIGALRM, sig_final);
		alarm(1);
	}
}
