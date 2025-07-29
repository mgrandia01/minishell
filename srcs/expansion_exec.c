/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/26 15:27:44 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Initialize expansion state (quote type, index, result buffer, etc.)
void	init_exp_data(t_exp_data *data, char first_char, char *env[])
{
	data->quote = get_quoted_type(first_char);
	data->i = 0;
	data->result = NULL;
	data->s = 0;
	data->env = env;
}

// Loop through token string and process variable expansions or literals
void	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_exp_data *data)
{
	char	*literal;

	while (t_val[data->i] != '\0')
	{
		if (t_val[data->i] == '$' && data->quote != 1)
		{
			if (data->s > 0)
			{
				literal = literal_tok(&(data->result), &(data->s));
				add_token(n_lst, c->type, literal, 1);
			}
			if (t_val[data->i + 1] == '?')
				handle_exit_status(&(data->i));
			else
			{
				data->result = exp_var_at_index(t_val, &(data->i), data->env);
				handle_exp_result(n_lst, c, &(data->result), data->quote);
				data->s = 0;
			}
		}
		else
			handle_literal_char(t_val, &(data->i), &(data->result), &(data->s));
	}
}

// Add final literal token if any characters were left unprocessed
void	finalize_expansion(t_token **n_lst, t_token *c, t_exp_data *data)
{
	if (data->s > 0)
		add_token(n_lst, c->type, literal_tok(&(data->result), &(data->s)), 0);
}
