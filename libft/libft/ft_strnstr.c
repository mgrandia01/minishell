/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 16:24:37 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 18:45:07 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	temp;

	i = 0;
	temp = 0;
	if (*little == '\0')
		return ((char *)big);
	while (temp < len && big[temp] != '\0')
	{
		if (little[i] == big[temp])
		{
			while (big[temp + i] == little[i]
				&& (temp + i) < len && little[i] != '\0')
				i++;
			if (i == (size_t)ft_strlen(little))
				return ((char *)&big[temp]);
			i = 0;
		}
		temp++;
	}
	return (NULL);
}
