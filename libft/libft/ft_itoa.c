/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:52:15 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 18:36:31 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static int	ft_lenint(int n)
{
	int			i;
	long int	num;

	i = 0;
	num = n;
	if (num == 0)
		i = 1;
	if (num < 0)
	{
		num = num * -1;
		i++;
	}
	while (num > 0)
	{
		num = num / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char		*ptr;
	int			i;
	long int	num;

	num = n;
	i = ft_lenint(num);
	ptr = (char *)malloc(i + 1 * sizeof(char));
	if (!ptr)
		return (NULL);
	ptr[i] = '\0';
	i = i - 1;
	if (num == 0)
		ptr[0] = '0';
	if (num < 0)
	{
		num = -num;
		ptr[0] = '-';
	}
	while (num > 0)
	{
		ptr[i] = num % 10 + '0';
		i --;
		num = num / 10;
	}
	return (ptr);
}
