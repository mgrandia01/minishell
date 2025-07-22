/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:56:11 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/14 14:46:06 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * Creates a new token and adds it to the end of the token list.
 * Returns 1 on success, 0 on failure.
 */
int	add_token(t_token **lst, t_token_type type, char *val, int end)
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

/*
 * Finds the content inside quotes. Returns the start position.
 * Returns -1 if the closing quote is not found.
 */
static int	process_quote_content(char *input, int *pos, char quote)
{
	int	start;

	start = *pos;
	while (input[*pos] && input[*pos] != quote)
		(*pos)++;
	if (!input[*pos])
	{
		ft_printf(STDERR_FILENO, "no se han cerrado las comillas");
		return (-1);
	}
	return (start);
}

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

	process_previous_word(input, list, data);
	quote = input[data->pos];
	data->pos++;
	start = process_quote_content(input, &data->pos, quote);
	if (start == -1)
		return (-1);
	if (input[data->pos] == quote)
		data->pos++;
	val = ft_substr(input, (start - 1), (data->pos + 1 - start));
	end = handle_end(input[data->pos]);
	add_token(list, TOKEN_WORD, val, end);
	return (0);
}

/*
 * Extracts and adds the previous word as a token if there is one.
 */
void	process_previous_word(char *input, t_token **list, t_pos_data *data)
{
	char	*val;
	int		end;

	if (data->start != data->pos)
	{
		val = ft_substr(input, data->start, data->pos - data->start);
		end = handle_end(input[data->pos]);
		add_token(list, TOKEN_WORD, val, end);
	}
}
