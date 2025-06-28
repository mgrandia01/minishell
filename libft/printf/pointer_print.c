/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pointer_print.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:00:40 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 18:33:40 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int	ft_print_pointer(char *ptr)
{
	int	size;

	if (ptr == NULL)
	{
		ft_print_string("(nil)");
		return (5);
	}
	size = 0;
	size += ft_print_string("0x");
	size += ft_print_hex_ptr((unsigned long)ptr, 'x');
	return (size);
}

char	*ft_conv_p_to_hex(unsigned long value, char type, int i)
{
	char				*str;
	unsigned int		r;

	str = malloc(2 * sizeof(unsigned long) + 1);
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

int	ft_print_hex_ptr(unsigned long value, char type)
{
	char	*p_str;
	int		size;
	int		i;

	i = 0;
	p_str = ft_conv_p_to_hex(value, type, i);
	if (!p_str)
		return (0);
	size = ft_print_string(p_str);
	free(p_str);
	return (size);
}
