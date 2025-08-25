/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:27:00 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/25 11:50:33 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//void	ft_add_history(char *input);

void	free_tokens(t_token *lst)
{
	t_token	*tmp;

	while (lst)
	{
		tmp = lst->next;
		if (lst->value)
			free(lst->value);
		free(lst);
		lst = tmp;
	}
}

/**
 * Processes special char (quotes, spaces, operators) at the current position.
 * Returns -1 on error, 0 otherwise.
 */
static int	process_special_chars(char *input, t_token **list, t_pos_data *data)
{
	if ((input[data->pos] == '\'') || (input[data->pos] == '\"'))
	{
		if (handle_quotes(input, list, data) == -1)
			return (-1);
		data->start = data->pos;
	}
	else if (data->state == 0 && (input[data->pos] == ' '
			|| input[data->pos] == '\t'))
		hand_whitespace(input, list, data);
	else if (input[data->pos] == '>' || input[data->pos] == '<'
		|| input[data->pos] == '|')
		handle_operators(input, list, data);
	else
		data->pos++;
	return (0);
}

static int	process_special_ch_heredoc(char *input, t_token **list, t_pos_data *data)
{

	if (data->state == 0 && (input[data->pos] == ' '
			|| input[data->pos] == '\t'))
		hand_whitespace(input, list, data);
	else if (input[data->pos] == '>' || input[data->pos] == '<'
		|| input[data->pos] == '|')
		handle_operators(input, list, data);
	else
		data->pos++;
	return (0);
}


/*
 * Tokenizes the input string and returns a list of tokens.
 * Returns NULL if an error occurs during tokenization.
 */
t_token	*ft_tokenize(char *input, int heredoc)
{
	t_token		*list;
	t_pos_data	data;

	init_tokenizer_data(&data, &list);
	while (input[data.pos])
	{
		if (heredoc == 0)
		{
			if (process_special_chars(input, &list, &data) == -1)
				return (NULL);
		}
		else
		{
			if (process_special_ch_heredoc(input, &list, &data) == -1)
				return (NULL);
		}
	}
	fin_tokenization(input, &list, &data);
	return (list);
}
