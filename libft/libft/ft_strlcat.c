/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:44:37 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 18:43:38 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	len_dest;
	size_t	len_src;

	if (size == 0)
		return (ft_strlen(src));
	len_dest = ft_strlen(dest);
	if (size <= len_dest)
		return (ft_strlen(src) + size);
	len_src = ft_strlcpy(dest + len_dest, src, size - len_dest);
	return (len_dest + len_src);
}
