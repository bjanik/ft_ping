/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 12:09:42 by bjanik            #+#    #+#             */
/*   Updated: 2020/07/27 12:09:43 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <stdio.h>

int		set_quiet(char **argv, int *index)
{
	(void)argv;
	(void)index;
	g_ping.opts.opt |= PING_QUIET;
	return (0);
}

int		set_socket_debug(char **argv, int *index)
{
	(void)argv;
	(void)index;
	g_ping.opts.opt |= PING_SOCKET_DEBUG;
	return (0);
}

int		set_count(char **argv, int *index)
{
	if (argv[*index + 1] == NULL)
	{
		dprintf(STDERR_FILENO, "ping: option requires an argument -- 'c'\n");
		return (1);
	}
	g_ping.opts.count = ft_atoi(argv[++(*index)]);
	if (g_ping.opts.count < 1 || g_ping.opts.count > LONG_MAX)
	{
		dprintf(STDERR_FILENO, "ping: invalid argument: '%s': ", argv[*index]);
		dprintf(STDERR_FILENO, "out of range: 0 <= value <= %ld\n", LONG_MAX);
		return (1);
	}
	return (0);
}

int		set_cap_d(char **argv, int *index)
{
	(void)argv;
	(void)index;
	g_ping.opts.opt |= PING_CAP_D;
	return (0);
}

int		set_data_size(char **argv, int *index)
{
	if (argv[*index + 1] == NULL)
	{
		dprintf(STDERR_FILENO, "ping: option requires an argument -- 's'\n");
		return (1);
	}
	g_ping.datalen = ft_atoi(argv[++(*index)]);
	if (g_ping.datalen < 0 || g_ping.datalen > 127992)
	{
		dprintf(STDERR_FILENO, "ping: invalid argument: '%s': ", argv[*index]);
		dprintf(STDERR_FILENO, "out of range: 0 <= value <= 127992\n");
		return (1);
	}
	return (0);
}

int		set_verbose(char **argv, int *index)
{
	(void)argv;
	(void)index;
	g_ping.opts.opt |= PING_VERBOSE;
	return (0);
}

static void set_options(struct s_ping_options options[])
{
	options[0] = {"-s", set_data_size};
	options[1] = {"-q", set_quiet};
	options[2] = {"-d", set_socket_debug};
	options[3] = {"-D", set_cap_d};
	options[4] = {"-c", set_count};
	options[5] = {"-v", set_verbose};
	options[6] = {NULL, NULL};
}

int		parse_options(int argc, char **argv)
{
	int	i;
	int	j;
	struct s_ping_options options[OPTIONS_LEN];


	(void)argc;
	i = 1;
	set_options(options);
	while (argv[i])
	{
		j = 0;
		if (argv[i][0] != '-')
			g_ping.dest_name = argv[i];
		while (g_ping.ping_options[j].opt)
		{
			if (ft_strcmp(g_ping.ping_options[j].opt, argv[i]) == 0)
			{
				if (g_ping.ping_options[j].opt_f(argv, &i))
					return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}
