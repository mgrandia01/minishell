/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:57:37 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 18:43:12 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t		i;
	size_t		j;
	char		*ptr;

	i = 0;
	j = 0;
	ptr = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1 * sizeof(char));
	if (!ptr)
		return (NULL);
	else
	{
		while (i < ft_strlen(s1))
		{
			ptr[i] = s1[i];
			i ++;
		}
		while (i < (ft_strlen(s1) + ft_strlen(s2)))
		{
			ptr[i] = s2[j];
			i ++;
			j ++;
		}
		ptr[i] = '\0';
	}
	return (ptr);
}
