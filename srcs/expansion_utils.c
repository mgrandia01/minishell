/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 12:10:49 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/26 13:02:09 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_words(const char *s)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && (s[i] == ' ' || s[i] == '\t'))
			i ++;
		if (s[i])
		{
			count ++;
			while (s[i] && (s[i] == ' ' || s[i] == '\t'))
				i ++;
		}
	}
	return (count);
}

static int	word_len(const char *s)
{
	int len;

	len = 0;
	while (s[len] && s[len] != ' ' && s[len] != '\t')
		len ++;
	return (len);
}

char	**ft_split_whitespace(char const *s)
{
	char	**result;
	int	words;
	int	i;
	int	len;

	i = 0;
	if (!s)
		return (NULL);
	words = count_words(s);
	result = malloc(sizeof(char *) * (words + 1));
	if (!result)
		return (NULL);
	while (*s)
	{
		while (*s && (*s == ' ' || *s == '\t'))
			s++;
		if (*s)
		{
			len = word_len(s);
			result[i++] = ft_substr(s, 0, len);
			s += len;
		}
	}
	result[i] = NULL;
	return (result);
}
