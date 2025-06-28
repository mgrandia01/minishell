/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 15:17:29 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 18:42:34 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strdup(const char *s)
{
	int		len;
	int		i;
	char	*ptr;
	char	*cs;

	cs = (char *)s;
	len = ft_strlen(cs);
	ptr = (char *) malloc((len + 1) * (sizeof(char)));
	i = 0;
	while (s[i] != '\0')
	{
		ptr[i] = s[i];
		i ++;
	}
	ptr[i] = '\0';
	return (ptr);
}
