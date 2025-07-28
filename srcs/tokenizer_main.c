/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/26 15:27:44 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Returns the quote type:
// 0 = no quote, 1 = single quote ('), 2 = double quote (")
static int	ft_quoted_type(char c)
{
	int	quote;

	quote = 0;
	if (c == '\'')
		quote = 1;
	else if (c == '"')
		quote = 2;
	return (quote);
}

// Splits a string by whitespace and adds each part as a token to the list.
// The original token type is preserved, and 'end' is set to 'e_fl' for the last token.
static void	spl_tok(t_token **n_lst, t_token_type type, char *t_exp, int e_fl)
{
	char	**split;
	int		i;
	int		end;
	char	*cleaned;

	if (!t_exp)
		return ;
	split = ft_split_whitespace(t_exp);
	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		end = 0;
		if (split[i + 1] == NULL)
			end = e_fl;
		cleaned = remove_quotes(split[i]);//FIXME es correcte el remove?
		if (cleaned)
			add_token(n_lst, type, cleaned, end);
		else
			add_token(n_lst, type, ft_strdup(split[i]), end);
		i++;
	}
	free_split_array(split);
}

static char	*literal_tok(char **result, int *size)
{
	char	*cleaned;

	*result = ft_realloc(*result, *size, *size + 1);
	(*result)[*size] = '\0';
	cleaned = remove_quotes(*result);//FIXME es correcte el remove?
	if (!cleaned)
		cleaned = ft_strdup(*result);
	free(*result);
	//no siempre, gestionar eso porque si es el fin si que es end, pero si es el inicio, no
	*result = NULL;
	*size = 0;
	return (cleaned);
}

static void	handle_exit_status(int *i)
{
	//FIXME Crec que podriem necessitar una variable global
	//TODO
	*i += 2;
}

static void	handle_literal_char(const char *t_val, int *i, char **r, int *s)
{
	*r = ft_realloc(*r, *s, *s + 1);
	(*r)[*s] = t_val[*i];
	(*s)++;
	(*i)++;
}

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
