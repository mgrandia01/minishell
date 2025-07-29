/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_process.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/26 15:27:44 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Process the result of a variable expansion: split if needed or clean quotes
void	handle_exp_result(t_token **n_lst, t_token *c, char **r, int q)
{
	char	*temp;

	if (ft_strchr(*r, ' ') && q == 0)
	{
		split_tok(n_lst, c->type, *r, 1);
		free(*r);
	}
	else
	{
		temp = *r;
		*r = remove_quotes(temp);
		if (!*r)
			*r = ft_strdup(temp);
		free(temp);
		add_token(n_lst, c->type, *r, 1);//FIXME why token = 1?
	}
	*r = NULL;
}

// Expand variables inside token value and add tokens to the new list
static void	exp_tok_val(const char *t_val, t_token **n_lst, t_token *c, char *env[])
{
	t_exp_data	data;

	init_exp_data(&data, t_val[0], env);
	p_exp(t_val, n_lst, c, &data);
	finalize_expansion(n_lst, c, &data);
}

// Go through all tokens and expand variables when needed
void	process_token_expansion(t_token **tokens, char *envp[])
{
	t_token		*new_list;
	t_token		*current;
	char		*cleaned_value;

	new_list = NULL;
	current = *tokens;
	while (current != NULL)
	{
		if (ft_strchr(current->value, '$'))
			exp_tok_val(current->value, &new_list, current, envp);
		else
		{
			cleaned_value = remove_quotes(current->value);
			if (!cleaned_value)
				cleaned_value = ft_strdup(current->value);
			add_token(&new_list, current->type, cleaned_value, current->end);
		}
		current = current->next;
	}
	free_tokens(*tokens);
	*tokens = new_list;
}
