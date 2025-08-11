/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/26 15:27:44 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

const char	*skip_space(const char *s)
{
	while (*s && (*s == ' ' || *s == '\t'))
		s++;
	return (s);
}

// Función para calcular la longitud de una palabra considerando comillas
int	word_len_with_quotes(const char *s)
{
	int		len;
	char	quote;

	len = 0;
	quote = 0;
	while (s[len])
	{
		if (!quote && (s[len] == '"' || s[len] == '\''))
		{
			quote = s[len];
			len++;
		}
		else if (quote && s[len] == quote)
		{
			quote = 0;
			len++;
			break ;
		}
		else if (!quote && (s[len] == ' ' || s[len] == '\t'))
			break ;
		else
			len++;
	}
	return (len);
}

// Función para contar palabras considerando comillas
int	count_words_with_quotes(const char *s)
{
	int	count;

	count = 0;
	s = skip_space(s);
	while (*s)
	{
		count++;
		s += word_len_with_quotes(s);
		s = skip_space(s);
	}
	return (count);
}

char	*extract_word(const char *s, int len)
{
	char	*word;
	int		i;
	int		j;

	if (len >= 2 && ((s[0] == '"' && s[len - 1] == '"')
			|| (s[0] == '\'' && s[len - 1] == '\'')))
	{
		word = malloc(len - 1);
		if (!word)
			return (NULL);
		i = 1;
		j = 0;
		while (i < len - 1)
		{
			word[j] = s[i];
			i++;
			j++;
		}
		word[j] = '\0';
	}
	else
	{
		word = ft_substr(s, 0, len);
	}
	return (word);
}

/*
 * Frees a NULL-terminated array of strings and its elements.
 */
void	free_split_array(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++ ;
	}
	free(split);
}
