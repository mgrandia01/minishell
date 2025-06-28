/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 09:34:10 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 18:33:55 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

char	*ft_flip_string(char *str)
{
	char	*flip;
	size_t	i;
	size_t	size;

	flip = ft_strdup(str);
	size = ft_strlen(str);
	i = 0;
	while (size > 0)
	{
		str[i] = flip [size - 1];
		i ++;
		size --;
	}
	free(flip);
	return (str);
}

int	ft_intlen(int i)
{
	int	size;

	size = 0;
	while (i > 9)
	{
		i = i / 10;
		size ++;
	}
	size ++;
	return (size);
}
