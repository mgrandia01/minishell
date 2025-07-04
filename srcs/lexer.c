/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:15:18 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/04 14:58:17 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_add_history(char *input);

//TODO Tu lexer debe detectar cadenas consecutivas entre comillas y concatenarlas si no hay espacios entre ellas.
//TODO  Tu lexer debería gestionar correctamente los escapes solo dentro de comillas dobles, y tratarlos como parte del string.
//TODO Tu lexer no debe crashear ni crear tokens vacíos si el input es vacío o contiene solo espacios.
//TODO echo hola"mundo"adio esto en bash es holamundoadios

//TODO No permitir cosas como | | o > sin un archivo después.
//TODO Manejo de variables y expansión
// Función auxiliar para procesar caracteres especiales
static int	process_special_chars(char *input, t_token **list, t_pos_data *data)
{
	if ((input[data->pos] == '\'') || (input[data->pos] == '\"'))
	{
		if (handle_quotes(input, list, &data->pos, &data->state) == -1)
			return (-1);
		data->start = data->pos;
	}
	else if (data->state == 0 && (input[data->pos] == ' '
			|| input[data->pos] == '\t'))
		handle_whitespace(input, list, data);
	else if (input[data->pos] == '>' || input[data->pos] == '<'
		|| input[data->pos] == '|')
		handle_operators(input, list, data);
	else
		data->pos++;
	return (0);
}

// Función principal de tokenización
t_token	*ft_tokenize(char *input)
{
	t_token		*list;
	t_pos_data	data;

	init_tokenizer_data(&data, &list);
	while (input[data.pos])
	{
		if (process_special_chars(input, &list, &data) == -1)
			return (NULL);
	}
	finalize_tokenization(input, &list, &data);
	return (list);
}
//struct s_cmd	*ft_parse(t_token *tokens);
