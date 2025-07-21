/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 09:29:00 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 16:49:05 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int	ft_select_format(va_list args, const char type, int fd)
{
	int	size;

	size = 0;
	if (type == 'c')
		size += ft_print_char(va_arg(args, int), fd);
	else if (type == 's')
		size += ft_print_string(va_arg(args, char *), fd);
	else if (type == 'p')
		size += ft_print_pointer(va_arg(args, char *), fd);
	else if (type == 'd' || type == 'i')
		size += ft_print_signed_int(va_arg(args, int), fd);
	else if (type == 'u')
		size += ft_print_unsigned_int(va_arg(args, unsigned int), fd);
	else if (type == 'x' || type == 'X')
		size += ft_print_hexadecimal((va_arg(args, int)), type, fd);
	else
		size += ft_print_char(type, fd);
	return (size);
}

int	ft_printf(int fd, char const *str, ...)
{
	va_list	args;
	int		i;
	int		size;

	i = 0;
	size = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			size += ft_select_format(args, str[i + 1], fd);
			i++;
		}
		else
			size += ft_print_char(str[i], fd);
		i++;
	}
	va_end(args);
	return (size);
}
