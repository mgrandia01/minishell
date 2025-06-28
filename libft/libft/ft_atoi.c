/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:51:45 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 16:48:28 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static int	ft_convert(const char *nptr, int i)
{
	int	n;
	int	res;

	n = 0;
	res = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		n = nptr[i] - '0';
		res = (res * 10) + n;
		i ++;
	}
	return (res);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sig;
	int	res;

	i = 0;
	sig = 1;
	res = 0;
	while (nptr[i] == ' ' || nptr[i] == '\t' || nptr[i] == '\v'
		|| nptr[i] == '\n'
		|| nptr[i] == '\r' || nptr[i] == '\f')
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sig = -1;
		i ++;
	}
	if (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = ft_convert(nptr, i);
		return (res * sig);
	}
	return (0);
}
