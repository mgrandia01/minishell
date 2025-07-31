/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 12:10:49 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/26 13:02:09 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * Counts the number of words in a string separated by spaces or tabs.
 */
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

/*
 * Returns the length of the next word until space or tab.
 */
static int	word_len(const char *s)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != ' ' && s[len] != '\t')
		len ++;
	return (len);
}

/*
 * Splits a string by whitespace (space or tab) into an array of words.
 * Returns a NULL-terminated array of strings.
 */
static char	**ft_split_whitespace(char const *s)
{
	char	**result;
	int		words;
	int		i;
	int		len;

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

// Splits a string by whitespace and adds each part as a token to the list.
// The original token type is preserved, and 'end' is set 
// to 'e_fl' for the last token.
void	split_tok(t_token **n_lst, t_token_type type, char *t_exp, int e_fl)
{
	char	**split;
	int		i;
	int		end;

	if (!t_exp)
		return ;
	split = ft_split_whitespace(t_exp);
	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		end = 0;
		if (split[i + 1] == NULL)
			end = e_fl;
		add_token(n_lst, type, ft_strdup(split[i]), end);
		i++;
	}
	free_split_array(split);
}
