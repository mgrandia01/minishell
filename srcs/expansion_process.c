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

static void	handle_exp_result(t_token **n_lst, t_token *c, char **r, int q)
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

//TODO Al llamar al add_token, no poner 1 en el end siempre, en el caso del
//final, respetar el end de lo que ya teniamos
// 0= no quote, 1 = '', 2= ""
/*void	exp_tok_val(const char *t_val, t_token **n_lst, t_token *c, char *env[])
{
	int		quote;
	int		i;
	char	*result;
	int		s;

	quote = get_quoted_type(t_val[0]);
	i = 0;
	result = NULL;
	s = 0;
	while (t_val[i] != '\0')
	{
		if (t_val[i] == '$' && quote != 1)
		{
			if (s > 0)
				add_token(n_lst, c->type, finalize_literal_tok(&result, &s), 1);
			if (t_val[i + 1] == '?')
				handle_exit_status(&i);
			else
			{
				result = exp_var_at_index(t_val, &i, env);
				handle_exp_result(n_lst, c, &result, quote);
				s = 0;
			}
		}
		else
			handle_literal_char(t_val, &i, &result, &s);
	}
	if (s > 0)
		add_token(n_lst, c->type, finalize_literal_tok(&result, &s), 0);
}*/
void    init_exp_data(t_exp_data *data, char first_char, char *env[])
{
    data->quote = get_quoted_type(first_char);
    data->i = 0;
    data->result = NULL;
    data->s = 0;
    data->env = env;
}

void    process_expansion(const char *t_val, t_token **n_lst, t_token *c, t_exp_data *data)
{
    while (t_val[data->i] != '\0')
    {
        if (t_val[data->i] == '$' && data->quote != 1)
        {
            if (data->s > 0)
                add_token(n_lst, c->type, finalize_literal_tok(&(data->result), &(data->s)), 1);
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

void    finalize_expansion(t_token **n_lst, t_token *c, t_exp_data *data)
{
    if (data->s > 0)
        add_token(n_lst, c->type, finalize_literal_tok(&(data->result), &(data->s)), 0);
}

void    exp_tok_val(const char *t_val, t_token **n_lst, t_token *c, char *env[])
{
    t_exp_data  data;
    
    init_exp_data(&data, t_val[0], env);
    process_expansion(t_val, n_lst, c, &data);
    finalize_expansion(n_lst, c, &data);
}
void	process_token_expansion(t_token **tokens, char *envp[])
{
   t_token	*new_list;
   t_token	*current;
   char	*cleaned_value;
   
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
