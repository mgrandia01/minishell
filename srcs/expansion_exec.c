/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/13 16:07:06 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	join_tokens_with_end(t_token **tokens)
{
	t_token	*curr;
	t_token	*next;
	char	*joined;

	curr = *tokens;
	while (curr && curr->next)
	{
		next = curr->next;
		while (next && curr->end == 1)
		{
			if (next->type == TOKEN_EOF)
				break ;
			joined = ft_strjoin(curr->value, next->value);
			free(curr->value);
			curr->value = joined;
			if (next->end == 1)
				curr->end = 1;
			curr->next = next->next;
			free(next->value);
			free(next);
			next = curr->next;
		}
		curr = curr->next;
	}
}

// Initialize expansion state (quote type, index, result buffer, etc.)
void	init_exp_data(t_dat *data, char first_char, char *env[])
{
	data->quote = get_quoted_type(first_char);
	data->i = 0;
	data->result = NULL;
	data->s = 0;
	data->env = env;
}

// Loop through token string and process variable expansions or literals
void	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_dat *d)
{
	char	*literal;

	while (t_val[d->i] != '\0')
	{
		if (t_val[d->i] == '$' && d->quote != 1)
		{
			if (d->s > 0)
			{
				literal = literal_tok(&(d->result), &(d->s));
				add_tok(n_lst, c->type, literal, 1);
			}
			if (t_val[d->i + 1] == '?')
			{
				if (*n_lst && ft_strncmp((*n_lst)->value, "echo", 5) == 0)
					handle_echo_exit_status(d);
				else
					handle_exit_status(&(d->i));
			}
			else
			{
				d->result = exp_var_at_index(t_val, &(d->i), d->env);
				handle_exp_result(n_lst, c, &(d->result), d->quote);
				d->s = 0;
			}
		}
		else
			handle_literal_ch(t_val, &(d->i), &(d->result), &(d->s));
	}
}

// Add final literal token if any characters were left unprocessed
void	finalize_expansion(t_token **n_lst, t_token *c, t_dat *data)
{
	if (data->s > 0)
		add_tok(n_lst, c->type, literal_tok(&(data->result), &(data->s)), 0);
}
