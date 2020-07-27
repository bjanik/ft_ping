/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/18 17:39:45 by bjanik            #+#    #+#             */
/*   Updated: 2017/02/07 15:57:51 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	print_string(t_arg *p, const int fd)
{
	int	len;

	len = p->arg_len;
	if (p->precision > -1 && p->precision < len)
		len = p->precision;
	if (p->width > len)
	{
		if (is_flag(p, '-') == 1)
		{
			ft_putnstr_fd(p->arg, len, fd);
			ft_putnchar_fd(' ', p->width - len, fd);
		}
		else
		{
			ft_putnchar_fd(' ', p->width - len, fd);
			ft_putnstr_fd(p->arg, len, fd);
		}
		return (p->width);
	}
	else
		ft_putnstr_fd(p->arg, len, fd);
	return (len);
}

static int	handle_special_case(t_arg *p, const int fd)
{
	if (p->spec == 'c' && p->arg[0] == '\0')
		return (ft_putchar_fd('\0', fd));
	if (ft_strchr("Oo", p->spec) != NULL && p->precision > -1 &&
			p->arg[0] == 0 && is_flag(p, '#') == 1)
		return (ft_putchar_fd('0', fd));
	return (0);
}

static int	print_number(t_arg *p, int nb_printed, const int fd)
{
	if (is_flag(p, '0') == 1)
	{
		nb_printed += ft_putnchar_fd('+', p->len.plus, fd);
		nb_printed += ft_putnchar_fd(' ', p->len.space, fd);
		nb_printed += (p->precision < 0) ? ft_putnchar_fd('-', p->neg, fd) : 0;
		if (p->arg[0] != 0 && p->precision == -1)
			nb_printed += ft_putstr_fd(p->print_hashtag, fd);
	}
	nb_printed += (is_flag(p, '-') == 0) ? ft_putstr_fd(p->print_width, fd) : 0;
	if (is_flag(p, '0') == 0)
	{
		nb_printed += ft_putnchar_fd('+', p->len.plus, fd);
		nb_printed += ft_putnchar_fd('-', p->neg, fd);
		nb_printed += ft_putnchar_fd(' ', p->len.space, fd);
		nb_printed += (p->arg[0] != 0) ? ft_putstr_fd(p->print_hashtag, fd) : 0;
	}
	if (p->precision > -1 && is_flag(p, '0') == 1)
		nb_printed += ft_putstr_fd(p->print_hashtag, fd);
	if (p->precision > -1 && is_flag(p, '0') == 1)
		nb_printed += ft_putnchar_fd('-', p->neg, fd);
	nb_printed += ft_putstr_fd(p->print_precision, fd);
	nb_printed += ft_putstr_fd(p->arg + p->neg, fd);
	nb_printed += handle_special_case(p, fd);
	nb_printed += (is_flag(p, '-') == 1) ? ft_putstr(p->print_width) : 0;
	return (nb_printed);
}

int			print_output(t_arg *p, const char *format, const int fd)
{
	int	nb_printed;

	nb_printed = 0;
	if (p == NULL)
		return (ft_putstr_fd(format, fd));
	while (p)
	{
		init_print(p);
		nb_printed += write(fd, p->start_format_part, p->end_format_part -
					p->start_format_part + 1);
		if (p->spec == 's' && p->lenght[0] != 'l')
			nb_printed += print_string(p, fd);
		else if (p->spec == 's')
			nb_printed += print_ls(p, fd);
		else if (p->spec == 'S')
			nb_printed += print_wide_s(p, fd);
		else if (p->spec == 'C' || (p->spec == 'c' && p->lenght[0] == 'l'))
			nb_printed += print_wide_c(p, fd);
		else
			nb_printed = print_number(p, nb_printed, fd);
		if (p->next == NULL)
			nb_printed += write(fd, p->end_format, ft_strlen(p->end_format));
		p = p->next;
	}
	return (nb_printed);
}
