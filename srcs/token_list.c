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
int	add_token(t_token **lst, t_token_type type, char *val, int end) //int quote)
{
	t_token	*new;

	new = malloc(sizeof(*new));
	if (!new || (type == TOKEN_WORD && !val))
		return (0);
	new->type = type;
	//new->value = ft_strdup(val);
	new->value = val;
	//new->quote = quote;
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
 * Updates and returns the quote parsing state based on the input character.
 * Toggles state for single and double quotes.
 
static int	ft_get_state(char input, int state)
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
*/
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

/**
 * Handles quoted strings, adds them as a token.
 * Returns -1 on error, 0 otherwise.
 */
//TODO guardar con las comillas para luego manejar, y no con el char quote
//guardar finalizado o no cada argumento para arreglar lo de --inlcude"*.c"
int	handle_quotes(char *input, t_token **list, int *pos, t_pos_data *data)// int *state)
{
	int		start;
	char	*val;
	char	quote;

	process_previous_word(input, list, data);//TODO poder podria passarli aqui el 1?
	quote = input[*pos];
	//*state = ft_get_state(input[*pos], *state);
	(*pos)++;
	start = process_quote_content(input, pos, quote);
	if (start == -1)
		return (-1);
	val = ft_substr(input, (start - 1), (*pos + 2 - start));// incluyendo las comillas
	add_token(list, TOKEN_WORD, val, 0);//TODO //*state);
	if (input[*pos] == quote)
	{
		//*state = ft_get_state(input[*pos], *state);
		(*pos)++;
	}
	return (0);
}

/*
 * Extracts and adds the previous word as a token if there is one.
 */
void	process_previous_word(char *input, t_token **list, t_pos_data *data)
{
	char	*val;
	int	end;
	char	c;

	c = input[data->pos];
	if (data->start != data->pos)
	{
		val = ft_substr(input, data->start, data->pos - data->start);
		if (((c == ' ') || (c == '\t') || (c == '\0')))//FIXME
			end = 0;
		else if ((c == '>') || (c == '<') || (c == '|'))//FIXME
			end = 0;
		else
			end = 1;

		add_token(list, TOKEN_WORD, val, end);// data->state);
	}
}
