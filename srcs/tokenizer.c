/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/26 15:27:44 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Returns the quote type:
// 0 = no quote, 1 = single quote ('), 2 = double quote (")
int	get_quoted_type(char c)
{
	int	quote;

	quote = 0;
	if (c == '\'')
		quote = 1;
	else if (c == '"')
		quote = 2;
	return (quote);
}

char	*finalize_literal_tok(char **result, int *size)
{
	char	*cleaned;

	*result = ft_realloc(*result, *size, *size + 1);
	(*result)[*size] = '\0';
	cleaned = remove_quotes(*result);//FIXME es correcte el remove?
	if (!cleaned)
		cleaned = ft_strdup(*result);
	free(*result);
	//no siempre, gestionar eso porque si es el fin si que es end, pero si es el inicio, no
	*result = NULL;
	*size = 0;
	return (cleaned);
}

void	handle_exit_status(int *i)
{
	//FIXME Crec que podriem necessitar una variable global
	//TODO
	*i += 2;
}

void	handle_literal_char(const char *t_val, int *i, char **r, int *s)
{
	*r = ft_realloc(*r, *s, *s + 1);
	(*r)[*s] = t_val[*i];
	(*s)++;
	(*i)++;
}
