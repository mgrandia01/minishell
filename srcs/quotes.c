/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/26 15:27:44 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * Finds the content inside quotes. Returns the start position.
 * Returns -1 if the closing quote is not found.
 */
int	process_q_cont(char *i, int *pos, char quote, t_token **tokens)
{
	int	start;

	start = *pos;
	while (i[*pos] && i[*pos] != quote)
		(*pos)++;
	if (!i[*pos])
	{
		ft_printf(STDERR_FILENO, "quotes not closed");//TODO igual que bash
		free_tokens(*tokens);
		return (-1);
	}
	return (start);
}

char	*remove_quotes(char *str)
{
	char	*result;
	int	i;
	int	j;
	char	quote;
	
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '"'))
			quote = str[i];
		else if (quote && str[i] == quote)
			quote = 0;
		else
			result[j++] = str[i];
		i ++;
	}
	result[j] = '\0';
	return (result);
}

void	remove_quotes_from_token_list(t_token *token)
{
	char	*cleaned;

	while (token)
	{
		cleaned = remove_quotes(token->value);
		free(token->value);
		token->value = cleaned;
		token = token->next;
	}
}
