/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   white_space.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:04:09 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/15 16:04:16 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * Advances position past consecutive whitespace characters.
 */
void	skip_whitespace(char *input, t_pos_data *data)
{
	while (input[data->pos] == ' ' || input[data->pos] == '\t')
		data->pos++;
}

/*
 * Handles whitespace by processing the previous word and skipping spaces.
 */
void	handle_whitespace(char *input, t_token **list, t_pos_data *data)
{
	process_previous_word(input, list, data);
	skip_whitespace(input, data);
	data->start = data->pos;
}

/*
 * Initializes tokenizer state and token list.
 */
void	init_tokenizer_data(t_pos_data *data, t_token **list)
{
	data->state = 0;
	data->pos = 0;
	data->start = 0;
	*list = NULL;
}

/*
 * Finalizes tokenization by processing any remaining word and adding EOF token
 */
void	finalize_tokenization(char *input, t_token **list, t_pos_data *data)
{
	process_previous_word(input, list, data);
	add_token(list, TOKEN_EOF, ft_strdup("EOF"), 0);
}
