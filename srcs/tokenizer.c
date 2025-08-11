/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/26 15:27:44 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Return quote type: 1 for single quote, 2 for double quote, 0 otherwise
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

// Return quote type: 1 for single quote, 2 for double quote, 0 otherwise
char	*literal_tok(char **result, int *size)
{
	*result = ft_realloc(*result, *size, *size + 1);
	(*result)[*size] = '\0';
	return (*result);
}

void	handle_exit_status(int *i)
{
	//FIXME Crec que podriem necessitar una variable global
	//TODO
	*i += 2;
}

// Add current char to result string and update indices
void	handle_literal_char(const char *t_val, int *i, char **r, int *s)
{
	*r = ft_realloc(*r, *s, *s + 1);
	(*r)[*s] = t_val[*i];
	(*s)++;
	(*i)++;
}
