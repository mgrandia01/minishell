/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 16:24:37 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 18:45:42 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static int	ft_find(char const *s1, char const *set, int order, size_t i)
{
	size_t	j;

	j = 0;
	if (order != 0)
		i = ft_strlen(s1) - 1;
	while (s1[i] != '\0')
	{
		while (set[j] != '\0')
		{
			if (s1[i] == set[j])
				break ;
			else
				j ++;
		}
		if (j == ft_strlen(set))
			return (i);
		if (i == ft_strlen(s1))
			return (0);
		if (order == 0)
			i++;
		else
			i--;
		j = 0;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char		*ptr;
	int			f_pos;
	int			l_pos;
	int			i;

	if (!s1 || !set)
		return (NULL);
	f_pos = ft_find(s1, set, 0, 0);
	l_pos = ft_find(s1, set, 1, 0);
	i = 0;
	ptr = (char *)malloc((l_pos - f_pos) + 2 * sizeof(char));
	if (!ptr)
		return (NULL);
	while ((i + f_pos) <= l_pos)
	{
		ptr[i] = s1[i + f_pos];
		i ++;
	}
	if (f_pos == 0 && l_pos == 0)
		ptr[i - 1] = '\0';
	ptr[i] = '\0';
	return (ptr);
}
