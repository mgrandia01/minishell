/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   white_space.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:04:09 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/04 14:11:39 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Función auxiliar para saltar espacios
void	skip_whitespace(char *input, t_pos_data *data)
{
	while (input[data->pos] == ' ' || input[data->pos] == '\t')
		data->pos++;
}

// Función para manejar espacios en blanco
void	handle_whitespace(char *input, t_token **list, t_pos_data *data)
{
	process_previous_word(input, list, data);
	skip_whitespace(input, data);
	data->start = data->pos;
}

// Función auxiliar para inicializar datos
void	init_tokenizer_data(t_pos_data *data, t_token **list)
{
	data->state = 0;
	data->pos = 0;
	data->start = 0;
	*list = NULL;
}

// Función auxiliar para finalizar tokenización
void	finalize_tokenization(char *input, t_token **list, t_pos_data *data)
{
	process_previous_word(input, list, data);
	add_token(list, TOKEN_EOF, "EOF", 0);
}
