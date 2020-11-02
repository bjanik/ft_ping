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

int 	set_ttl(char **argv, int *index)
{
	if (argv[*index + 1] == NULL)
	{
		dprintf(STDERR_FILENO, "ping: option requires an argument -- 's'\n");
		return (1);
	}
	g_ping.ttl = ft_atoi(argv[++(*index)]);
	if (g_ping.ttl < 0 || g_ping.datalen > 255)
	{
		dprintf(STDERR_FILENO, "ping: invalid argument: '%s': ", argv[*index]);
		dprintf(STDERR_FILENO, "out of range: 0 <= value <= 127992\n");
		return (1);
	}
	return (0);
}

int		parse_options(int argc, char **argv)
{
	int	i, j;
	struct s_ping_options options[] = {
		{"-c", set_count},
		{"-D", set_cap_d},
		{"-d", set_socket_debug},
		{"-q", set_quiet},
		{"-s", set_data_size},
		{"-t", set_ttl},
		{"-v", set_verbose},
		{NULL, NULL}
	};

	(void)argc;
	i = 1;
	while (argv[i])
	{
		j = 0;
		if (argv[i][0] != '-')
		{
			g_ping.dest_name = argv[i];
			return (0);
		}
		while (options[j].opt != NULL)
		{
			if (ft_strcmp(options[j].opt, argv[i]) == 0)
			{
				if (options[j].opt_f(argv, &i))
					return (1);
				break;
			}
			j++;
		}
		if (options[j].opt == NULL)
		{
			dprintf(STDERR_FILENO, "ft_ping: invalid option -- '%c'\n", argv[i][1]);
			return (ft_ping_usage());
		}
		i++;
	}
	return (0);
}
