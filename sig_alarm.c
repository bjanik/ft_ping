#include "ft_ping.h"

void sig_alarm(int x)
{
	(void)x;
	if (g_ping.ipv == IPV4)
		send_ipv4();
	else
		send_ipv6();
	if (g_ping.opts.count == 0 || g_ping.transmitted < g_ping.opts.count)
		alarm(1);
	else
	{
		signal(SIGALRM, sig_final);
		alarm(1);
		// sig_final(0);
	}
}