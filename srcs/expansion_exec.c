/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/30 11:38:34 by mgrandia         ###   ########.fr       */
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
			if (next->end == 0)
				curr->end = 0;
			curr->next = next->next;
			free(next->value);
			free(next);
			next = curr->next;
		}
		curr = curr->next;
	}
}

static int	end_value(const char *t_val, int i)
{
	int	end;

	end = 0;
	if (t_val[i] == '$' || (t_val[i] == ' '
			&& t_val[i + 1] != '$'))
		end = 1;
	return (end);
}

/// Loop through token string and process variable expansions or literals
void	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_dat *d)
{
	char	*literal;
	int		end;

	while (t_val[d->i] != '\0')
	{
		if (t_val[d->i] == '$' && d->quote != 1 && t_val[d->i + 1])
		{
			if (d->s > 0)
			{
				literal = literal_tok(&(d->result), &(d->s));
				add_tok(n_lst, c->type, literal, 1);
			}
			if (t_val[d->i + 1] == '?')
				handle_echo_exit_status(d);
			else
			{
				d->result = exp_var_at_index(t_val, &(d->i), d->env);
				end = end_value(t_val, d->i);
				handle_exp_res(n_lst, c, d, end);
				d->s = 0;
			}
		}
		else
			handle_literal_ch(t_val, &(d->i), &(d->result), &(d->s));
	}
}

void	p_all(const char *t_val, t_token **n_lst, t_token *c, t_dat *d)
{
	char	*literal;
	int		end;

	while (t_val[d->i] != '\0')
	{
		if (t_val[d->i] == '$' && t_val[d->i + 1])
		{
			if (d->s > 0)
			{
				literal = literal_tok(&(d->result), &(d->s));
				add_tok(n_lst, c->type, literal, 1);
			}
			if (t_val[d->i + 1] == '?')
				handle_echo_exit_status(d);
			else
			{
				d->result = exp_var_at_index(t_val, &(d->i), d->env);
				end = end_value(t_val, d->i);
				handle_exp_res(n_lst, c, d, end);
				d->s = 0;
			}
		}
		else
			handle_literal_ch(t_val, &(d->i), &(d->result), &(d->s));
	}
}

// Add final literal token if any characters were left unprocessed
void	finalize_expansion(t_token **n_lst, t_token *c, t_dat *d)
{
	if (d->s > 0)
		add_tok(n_lst, c->type, literal_tok(&(d->result), &(d->s)), c->end);
}
