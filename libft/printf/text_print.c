/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_print.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 09:28:34 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 18:34:09 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

int	ft_print_char(char c)
{
	write (1, &c, 1);
	return (1);
}

int	ft_print_string(char *s)
{
	int	size;

	if (s == NULL)
	{
		ft_print_string("(null)");
		return (6);
	}
	size = 0;
	while (s[size])
	{
		size += ft_print_char(s[size]);
	}
	return (size);
}
