/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hexa_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:00:40 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 18:33:25 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int	ft_intlen_hex(unsigned long value)
{
	int	len;

	len = 0;
	if (value == 0)
		return (1);
	while (value > 0)
	{
		value = value / 16;
		len ++;
	}
	return (len);
}

char	ft_letter_hexa(int value, char type)
{
	char	let;

	if (value == 10)
		let = 'A';
	else if (value == 11)
		let = 'B';
	else if (value == 12)
		let = 'C';
	else if (value == 13)
		let = 'D';
	else if (value == 14)
		let = 'E';
	else if (value == 15)
		let = 'F';
	if (type == 'x')
		return (ft_tolower(let));
	else if (type == 'X')
		return (let);
	else
		return (0);
}

char	*ft_convert_to_hex(unsigned int value, char type, int i)
{
	char				*str;
	unsigned int		r;

	str = malloc(ft_intlen_hex(value) + 1);
	r = 0;
	if (!str)
		return (NULL);
	if (value == 0)
	{
		str[i] = '0';
		str[i + 1] = '\0';
		return (str);
	}
	while (value > 0)
	{
		r = value % 16;
		if (r > 9)
			str[i] = ft_letter_hexa(r, type);
		else
			str[i] = r + '0';
		value = value / 16;
		i ++;
	}
	str[i] = '\0';
	return (ft_flip_string(str));
}

int	ft_print_hexadecimal(unsigned int value, char type)
{
	char	*hex_str;
	int		size;
	int		i;

	i = 0;
	hex_str = ft_convert_to_hex(value, type, i);
	if (!hex_str)
		return (0);
	size = ft_print_string(hex_str);
	free(hex_str);
	return (size);
}
