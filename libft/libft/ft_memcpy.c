/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:52:00 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 18:40:07 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*des;
	unsigned char	*ori;

	if (dest == NULL && src == NULL && n != 0)
		return (0);
	i = 0;
	des = (unsigned char *)dest;
	ori = (unsigned char *)src;
	while (i < n)
	{
		des[i] = ori[i];
		i++;
	}
	return (dest);
}
