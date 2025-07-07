/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:56:11 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/07 13:13:09 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	add_token(t_token **lst, t_token_type type, char *val, int quote)
{
	t_token	*new;

	new = malloc(sizeof(*new));
	if (!new || (type == TOKEN_WORD && !val))
		return (0);
	new->type = type;
	new->value = ft_strdup(val);
	new->quote = quote;
	new->next = NULL;
	while (*lst && (*lst)->next)
		lst = &(*lst)->next;
	if (*lst)
		(*lst)->next = new;
	else
		*lst = new;
	return (1);
}

int	ft_get_state(char input, int state)
{
	if (input == '\'' && state == 0)
		state = 1;
	else if (input == '\'' && state == 1)
		state = 0;
	else if (input == '"' && state == 0)
		state = 2;
	else if (input == '"' && state == 2)
		state = 0;
	return (state);
}

// Función auxiliar para procesar contenido entre comillas
int	process_quote_content(char *input, int *pos, char quote)
{
	int	start;

	start = *pos;
	while (input[*pos] && input[*pos] != quote)
		(*pos)++;
	if (!input[*pos])
	{
		ft_printf("no se han cerrado las comillas");
		return (-1);
	}
	return (start);
}

// Función para manejar comillas (simples y dobles)
int	handle_quotes(char *input, t_token **list, int *pos, int *state)
{
	int		start;
	char	*val;
	char	quote;

	quote = input[*pos];
	*state = ft_get_state(input[*pos], *state);
	(*pos)++;
	start = process_quote_content(input, pos, quote);
	if (start == -1)
		return (-1);
	val = ft_substr(input, start, *pos - start);
	add_token(list, TOKEN_WORD, val, *state);
	if (input[*pos] == quote)
	{
		*state = ft_get_state(input[*pos], *state);
		(*pos)++;
	}
	return (0);
}

// Función auxiliar para procesar texto previo
void	process_previous_word(char *input, t_token **list, t_pos_data *data)
{
	char	*val;

	if (data->start != data->pos)
	{
		val = ft_substr(input, data->start, data->pos - data->start);
		add_token(list, TOKEN_WORD, val, data->state);
	}
}
