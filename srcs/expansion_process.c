/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_process.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/28 10:51:59 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Initialize expansion state (quote type, index, result buffer, etc.)
void	init_exp_data(t_dat *data, char first_char, char *env[])
{
	data->quote = get_quoted_type(first_char);
	data->i = 0;
	data->result = NULL;
	data->s = 0;
	data->env = env;
}

// Process the result of a variable expansion: split if needed or clean quotes
void	handle_exp_result(t_token **n_lst, t_token *c, t_dat *d, int end)
{
	if (ft_strchr(d->result, ' ') && d->quote == 0)
	{
		split_tok(n_lst, c->type, d->result, c->end);
		free(d->result);
	}
	else
		add_tok(n_lst, c->type, d->result, end);
	d->result = NULL;
}

// Expand variables inside token value and add tokens to the new list
static void	exp_tok_val(const char *val, t_token **n_l, t_token *c, char *env[])
{
	t_dat	data;

	init_exp_data(&data, val[0], env);
	p_exp(val, n_l, c, &data);
	finalize_expansion(n_l, c, &data);
}

static void	exp_tok_her(const char *val, t_token **n_l, t_token *c, char *env[])
{
	t_dat	data;

	init_exp_data(&data, val[0], env);
	p_exp_all(val, n_l, c, &data);
	finalize_expansion(n_l, c, &data);
}

// Go through all tokens and expand variables when needed
void	process_token_expansion(t_token **tokens, char *envp[], int here)
{
	t_token		*new_list;
	t_token		*c;

	new_list = NULL;
	c = *tokens;
	while (c != NULL)
	{
		if (ft_strchr(c->value, '$'))
		{
			if (here == 0)
				exp_tok_val(c->value, &new_list, c, envp);
			else
				exp_tok_her(c->value, &new_list, c, envp);
		}
		else
			add_tok(&new_list, c->type, ft_strdup(c->value), c->end);
		c = c->next;
	}
	free_tokens(*tokens);
	*tokens = new_list;
}
