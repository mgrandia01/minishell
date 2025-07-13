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

// Función para manejar redirección de salida (> y >>)
void	handle_output_redir(char *input, t_token **list, int *pos, int state)
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

// Función para manejar redirección de entrada (< y <<)
void	handle_input_redir(char *input, t_token **list, int *pos, int state)
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

// Función para manejar pipe
void	handle_pipe(t_token **list, int *pos, int state)
{
	add_token(list, TOKEN_PIPE, ft_strdup("|"), state);
	(*pos)++;
}

// Función para procesar operadores
void	process_operator(char *input, t_token **list, t_pos_data *data)
{
	if (input[data->pos] == '>')
		handle_output_redir(input, list, &data->pos, data->state);
	else if (input[data->pos] == '<')
		handle_input_redir(input, list, &data->pos, data->state);
	else if (input[data->pos] == '|')
		handle_pipe(list, &data->pos, data->state);
}

// Función para manejar operadores especiales
void	handle_operators(char *input, t_token **list, t_pos_data *data)
{
	process_previous_word(input, list, data);
	process_operator(input, list, data);
	data->start = data->pos;
}
