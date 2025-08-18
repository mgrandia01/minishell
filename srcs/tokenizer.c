/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/18 17:01:31 by mgrandia         ###   ########.fr       */
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
	ft_printf(STDERR_FILENO, "minishell: %d: command not found\n", g_exit_status);
	*i += 2;
}

void	handle_echo_exit_status(t_dat *data)
{
	char	*tmp;
	int		j;

	tmp = ft_itoa(g_exit_status);
	j = 0;
	while (tmp[j])
		handle_literal_ch(tmp, &j, &(data->result), &(data->s));
	free (tmp);
	data->i + =2;
}

// Add current char to result string and update indices
void	handle_literal_ch(const char *t_val, int *i, char **r, int *s)
{
	*r = ft_realloc(*r, *s, *s + 1);
	(*r)[*s] = t_val[*i];
	(*s)++;
	(*i)++;
}
