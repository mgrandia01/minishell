/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/13 15:51:53 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	**ft_split_whitespace(char const *s, int i)
{
	char	**result;
	int		words;
	int		len;
	int		debug;

	if (!s)
		return (NULL);
	words = count_words_with_quotes(s);
	result = malloc(sizeof(char *) * (words + 1));
	if (!result)
		return (NULL);
	s = skip_space(s);
	while (*s && i < words)
	{
		len = word_len_with_quotes(s);
		debug = 0;
		while (debug < len)
			debug++;
		result[i] = extract_word(s, len);
		i++;
		s += len;
		s = skip_space(s);
	}
	result[i] = NULL;
	return (result);
}

void	split_tok(t_token **n_lst, t_token_type t, char *t_e, int e_fl)
{
	char	**split;
	int		i;
	int		end;
	int		debug;

	if (!t_e)
		return ;
	split = ft_split_whitespace(t_e, 0);
	if (!split)
		return ;
	debug = 0;
	while (split[debug])
		debug++;
	i = 0;
	while (split[i])
	{
		end = 0;
		if (split[i + 1] == NULL)
			end = e_fl;
		add_tok(n_lst, t, ft_strdup(split[i]), end);
		i++;
	}
	free_split_array(split);
}
