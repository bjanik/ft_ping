/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 12:11:34 by bjanik            #+#    #+#             */
/*   Updated: 2020/07/27 12:11:38 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H
# include <arpa/inet.h>
# include <linux/sockios.h>
# include <linux/icmp.h>
# include <linux/ip.h>
# include <limits.h>
# include <netdb.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <unistd.h>

# define TIMEVAL_SIZE 16
# define DEFAULT_DATALEN 56

# define IPV4 0
# define PING_QUIET 1
# define PING_SOCKET_DEBUG 2
# define PING_VERBOSE 4
# define PING_CAP_D 8
# define OPTIONS_LEN 7

struct						s_ping_options
{
	char					*opt;
	int						(*opt_f)(char **argv, int *index);
};

struct						s_opts
{
	int						count;
	int						opt;
};

struct						s_ping
{
	struct sockaddr_in		dest;
	char					*dest_name;
	char					*canonname;
	char					*ipstr;
	int						ipv;
	int						socket_fd;
	int						transmitted;
	int						received;
	pid_t					id;
	int						timing;
	int						datalen;
	float					rtt_sum;
	float					rtt_max;
	float					rtt_min;
	struct timeval			start;
	struct s_opts			opts;
};

int							send_ipv4(void);
int							recv_pong(void);

unsigned short				in_cksum(void *ptr, int len);
void						ft_tv_sub(struct timeval *tv, struct timeval *time);

void						sig_alarm(int x);
void						sig_final(int x);

int							set_quiet(char **argv, int *index);
int							set_socket_debug(char **argv, int *index);
int							set_count(char **argv, int *index);
int							set_cap_d(char **argv, int *index);
int							set_data_size(char **argv, int *index);
int							set_verbose(char **argv, int *index);
int							parse_options(int argc, char **argv);

struct s_ping				g_ping;

#endif
