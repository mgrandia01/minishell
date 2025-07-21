/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:46:55 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/04 14:10:49 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Adds output redirection or append token based on input characters.
 * Advances the input position accordingly.
 */
static void	handle_output_redir(char *input, t_token **list, int *pos, int state)
{
	if (input[*pos + 1] == '>')
	{
		add_token(list, TOKEN_REDIR_APPEND, ft_strdup(">>"), state);
		*pos += 2;
	}
	else
	{
		add_token(list, TOKEN_REDIR_OUT, ft_strdup(">"), state);
		(*pos)++;
	}
}

/* Adds input redirection or heredoc token based on the input characters.
 * Advances the input position accordingly.
 */
static void	handle_input_redir(char *input, t_token **list, int *pos, int state)
{
	if (input[*pos + 1] == '<')
	{
		add_token(list, TOKEN_HEREDOC, ft_strdup("<<"), state);
		*pos += 2;
	}
	else
	{
		add_token(list, TOKEN_REDIR_IN, ft_strdup("<"), state);
		(*pos)++;
	}
}

/* Adds a pipe token to the list and advances the input position.
 */
static void	handle_pipe(t_token **list, int *pos, int state)
{
	add_token(list, TOKEN_PIPE, ft_strdup("|"), state);
	(*pos)++;
}

/* Processes a single operator token: output redirection,
 * input redirection, or pipe, based on the current input char.
 */
static void	process_operator(char *input, t_token **list, t_pos_data *data)
{
	if (input[data->pos] == '>')
		handle_output_redir(input, list, &data->pos, data->state);
	else if (input[data->pos] == '<')
		handle_input_redir(input, list, &data->pos, data->state);
	else if (input[data->pos] == '|')
		handle_pipe(list, &data->pos, data->state);
}

/* Processes operators by handling the previous word
 * and then the operator itself. Updates tokenizer position.
 */
void	handle_operators(char *input, t_token **list, t_pos_data *data)
{
	process_previous_word(input, list, data);
	process_operator(input, list, data);
	data->start = data->pos;
}
