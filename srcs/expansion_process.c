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

static void	process_exp_res(t_token **n_lst, t_token *c, char **r, int q)
{
	char	*temp;

	if (ft_strchr(*r, ' ') && q == 0)
	{
		spl_tok(n_lst, c->type, *r, 1);
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

//TODO Al llamar al add_token, no poner 1 en el end siempre, en el caso del
//final, respetar el end de lo que ya teniamos
// 0= no quote, 1 = '', 2= ""
void	exp_tok_val(const char *t_val, t_token **n_lst, t_token *c, char *env[])
{
	int		quote;
	int		i;
	char	*result;
	int		s;

	quote = ft_quoted_type(t_val[0]);
	i = 0;
	result = NULL;
	s = 0;
	while (t_val[i] != '\0')
	{
		if (t_val[i] == '$' && quote != 1)
		{
			if (s > 0)
				add_token(n_lst, c->type, literal_tok(&result, &s), 1);
			if (t_val[i + 1] == '?')
				handle_exit_status(&i);
			else
			{
				result = expand_variable_at(t_val, &i, env);
				process_exp_res(n_lst, c, &result, quote);
				s = 0;
			}
		}
		else
			handle_literal_char(t_val, &i, &result, &s);
	}
	if (s > 0)
		add_token(n_lst, c->type, literal_tok(&result, &s), 0);
}

void	process_expansion(t_token **tokens, char *envp[])
{
	t_token	*new_list;
	t_token	*current;
	char	*cleaned_value;

	new_list = NULL;
	current = *tokens;
	(void)envp;
	while (current != NULL)
	{
		if (ft_strchr(current->value, '$'))
			exp_tok_val(current->value, &new_list, current, envp);
		else
		{
			//TODO Gestionar para que no incluya las comillas
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
