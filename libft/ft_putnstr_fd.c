/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnstr_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/18 14:45:04 by bjanik            #+#    #+#             */
/*   Updated: 2017/01/13 17:22:29 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnstr_fd(const char *str, int n, const int fd)
{
	int	len;

	len = ft_strlen(str);
	if (n < len && n >= 0)
	{
		write(fd, str, n);
		return (n);
	}
	else if (n > 0)
		write(fd, str, len);
	return (len);
}
