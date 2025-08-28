/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:56:11 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/28 10:10:23 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/*
 * Creates a new token and adds it to the end of the token list.
 * Returns 1 on success, 0 on failure.
 */
int	add_tok(t_token **lst, t_token_type type, char *val, int end)
{
	t_token	*new;

	new = malloc(sizeof(*new));
	if (!new || (type == TOKEN_WORD && !val))
		return (0);
	new->type = type;
	new->value = val;
	new->end = end;
	new->next = NULL;
	if (!*lst)
	{
		*lst = new;
		return (1);
	}
	while (*lst && (*lst)->next)
		lst = &(*lst)->next;
	if (*lst)
		(*lst)->next = new;
	else
		*lst = new;
	return (1);
}

// Checks if the given character marks the end of a token (returns 0)
// or not (returns 1)
static int	handle_end(char c)
{
	int	end;

	if (((c == ' ') || (c == '\t') || (c == '\0')))
		end = 0;
	else if ((c == '>') || (c == '<') || (c == '|'))
		end = 0;
	else
		end = 1;
	return (end);
}

/**
 * Handles quoted strings, adds them as a token.
 * Returns -1 on error, 0 otherwise.
 */
int	handle_quotes(char *input, t_token **list, t_pos_data *data)
{
	int		start;
	char	*val;
	char	quote;
	int		end;

	process_pre_word(input, list, data);
	quote = input[data->pos];
	data->pos++;
	start = process_q_cont(input, &data->pos, quote, list);
	if (start == -1)
		return (-1);
	if (input[data->pos] == quote)
		data->pos++;
	val = ft_substr(input, (start - 1), (data->pos + 1 - start));
	end = handle_end(input[data->pos]);
	add_tok(list, TOKEN_WORD, val, end);
	return (0);
}

/*
 * Extracts and adds the previous word as a token if there is one.
 */
void	process_pre_word(char *input, t_token **list, t_pos_data *data)
{
	char	*val;
	int		end;

	if (data->start != data->pos)
	{
		val = ft_substr(input, data->start, data->pos - data->start);
		end = handle_end(input[data->pos]);
		add_tok(list, TOKEN_WORD, val, end);
	}
}
