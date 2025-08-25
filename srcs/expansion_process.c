/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_process.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/25 10:46:25 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Process the result of a variable expansion: split if needed or clean quotes
void	handle_exp_result(t_token **n_lst, t_token *c, char **r, int q)
{
	int	end;

	if (ft_strchr(*r, ' ') && q == 0)
	{
		split_tok(n_lst, c->type, *r, c->end);
		free(*r);
	}
	else
	{
		end = c->end;
		add_tok(n_lst, c->type, *r, end);
	}
	*r = NULL;
}

// Expand variables inside token value and add tokens to the new list
static void	exp_tok_val(const char *val, t_token **n_l, t_token *c, char *env[])
{
	t_dat	data;

	init_exp_data(&data, val[0], env);
	p_exp(val, n_l, c, &data);
	finalize_expansion(n_l, c, &data);
}

// Go through all tokens and expand variables when needed
void	process_token_expansion(t_token **tokens, char *envp[])
{
	t_token		*new_list;
	t_token		*c;

	new_list = NULL;
	c = *tokens;
	while (c != NULL)
	{
		if (ft_strchr(c->value, '$'))
			exp_tok_val(c->value, &new_list, c, envp);
		else
			add_tok(&new_list, c->type, ft_strdup(c->value), c->end);
		c = c->next;
	}
	free_tokens(*tokens);
	*tokens = new_list;
}
