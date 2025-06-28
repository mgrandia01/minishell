/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decimal_print.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 11:01:17 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 16:48:51 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int	ft_print_int(int i)
{
	int		size;
	char	*str;

	size = 0;
	str = ft_itoa(i);
	size += ft_print_string(str);
	free(str);
	return (size);
}

unsigned int	ft_print_unsigned_int(unsigned int i)
{
	int		size;
	char	*str;

	size = 0;
	str = ft_uitoa(i);
	size += ft_print_string(str);
	free (str);
	return (size);
}

int	ft_print_signed_int(int i)
{
	int		size;
	char	*str;

	size = 0;
	if (i >= 0)
		size += ft_print_int(i);
	else if (i == -2147483648)
	{
		str = ft_itoa(i);
		write(1, str, 11);
		size = 11;
		free (str);
	}
	else
	{
		size += ft_print_char('-');
		size += ft_print_int(-i);
	}
	return (size);
}
