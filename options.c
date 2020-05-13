#include "ft_ping.h"
#include <stdio.h>

struct s_ping_options
{
	char	*opt;
	int 	(*opt_f)(char **argv, int *index);
};

int 	set_ipv6(char **argv, int *index)
{
	(void)argv;
	(void)index;
	g_ping.opts.opt |= PING_IPV6;
	(*index)++;
	return (0);
}

int 	set_quiet(char **argv, int *index)
{
	(void)argv;
	(void)index;
	g_ping.opts.opt |= PING_QUIET;
	(*index)++;
	return (0);
}

int 	set_socket_debug(char **argv, int *index)
{
	(void)argv;
	(void)index;
	g_ping.opts.opt |= PING_SOCKET_DEBUG;
	(*index)++;
	return (0);	
}

int 	set_count(char **argv, int *index)
{
	if (argv[*index + 1] == NULL)
	{
		dprintf(STDERR_FILENO, "ping: option requires an argument -- 'c'\n");
		return (1);
	}
	g_ping.opts.count = atoi(argv[++(*index)]);
	return (0);
}

int 	set_cap_d(char **argv, int *index)
{
	(void)argv;
	(void)index;
	g_ping.opts.opt |= PING_CAP_D;
	return (0);
}

int 	set_data_size(char **argv, int *index)
{
	if (argv[*index + 1] == NULL)
	{
		dprintf(STDERR_FILENO, "ping: option requires an argument -- 's'\n");
		return (1);
	}
	g_ping.opts.packet_size = atoi(argv[++(*index)]);
	return (0);	
}

int 	set_verbose(char **argv, int *index)
{
	(void)argv;
	(void)index;
	g_ping.opts.opt |= PING_VERBOSE;
	return (0);
}

int 	parse_options(int argc, char **argv)
{
	int j, i;

	(void)argc;
	struct s_ping_options 	ping_options[] = {
		{"-6", set_ipv6},
		{"-c", set_count},
		{"-D", set_cap_d},
		{"-q", set_quiet},
		{"-s", set_data_size},
		{"-v", set_verbose},
		{NULL, NULL}
	};
	i = 1;
	while (argv[i])
	{
		j = 0;
		if (argv[i][0] != '-')
			g_ping.dest_name = argv[i];
		while (ping_options[j].opt)
		{
			if (strcmp(ping_options[j].opt, argv[i]) == 0)
			{
				if (ping_options[j].opt_f(argv, &i))
					return (1);
			}
			j++;
		}
		i++;
	}
	return (0);
}