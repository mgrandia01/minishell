/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:03:28 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 18:40:19 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*des;
	unsigned char	*ori;

	i = n;
	des = (unsigned char *)dest;
	ori = (unsigned char *)src;
	if (ori < des)
	{
		while (i > 0)
		{
			i--;
			des[i] = ori[i];
		}
	}
	else
		ft_memcpy(des, ori, n);
	return (dest);
}
