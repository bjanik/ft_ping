#include "ft_ping.h"

void sig_alarm(int x)
{
	send_ping();

	if (g_ping.opts.count == 0 || g_ping.transmitted < g_ping.opts.count)
		alarm(1);
}