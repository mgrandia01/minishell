/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:51:45 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 18:45:24 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char		ch;
	const char	*last_ch;

	ch = (char)c;
	last_ch = NULL;
	while (*s != '\0')
	{
		if (*s == ch)
		{
			last_ch = s;
		}
		s++;
	}
	if (ch == '\0')
		return ((char *)s);
	return ((char *)last_ch);
}
