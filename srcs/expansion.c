/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/26 15:27:44 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

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

static char	*remove_quotes(char *str)
{
	int	i;
	int	j;
	char	quote;
	char	*result;

	i = 0;
	j = 0;
	quote = 0;
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '\'') || (str[i] == '"'))
		{
			if (!quote)
				quote = str[i];
			else if (quote == str[i])
				quote = 0;
			i++;
			continue;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}


static char	*get_env_value(const char *name, char *envp[])
{
	int	i;
	size_t	len;

	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return(envp[i] + len + 1);
		i ++;
	}
	return (NULL);
}

static char	*expand_variable_at(const char *token_value, int *i, char *envp[])
{

	// TODO las variables creadas por el usuario

/*
** A partir de la posición `*i`, detecta y extrae el nombre de la variable. alfanumerico o _
** Busca el valor correspondiente en `envp`.
** Devuelve el valor como string (puede contener espacios).
** Avanza el índice `*i` hasta el final del nombre de la variable para que el bucle lo continúe bien.
*/
	char	*var_name;
	char	*value;
	int	start;
	int	len;

	start = *i;
	(*i)++;
	len = 0;
	while(token_value[*i+len] && (ft_isalnum(token_value[*i + len]) || token_value[*i + len] == '_')) 
		len++;
	var_name = ft_substr(token_value, start + 1, len);
	*i += len;
	if (!var_name)
		return (NULL);
	value = get_env_value(var_name, envp);
	free(var_name);
	if(!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

/*
** Devuelve una string que representa el valor de la última variable $? expandida.
** Este valor debe estar guardado globalmente o ser accesible de alguna forma.
*/
/*
static char	*expand_exit_status(void)
{
	//FIXME Crec que podriem necessitar una variable global
}*/

static void	split_and_add_token(t_token **new_list, t_token_type type, char *token_exp, int end_flag)
{
	char	**split;
	int	i;
	int	end;
	char	*cleaned;

	if (!token_exp)
		return ;
	split = ft_split_whitespace(token_exp);//TODO nueva funcion que divide por espacios y /t
	if (!split)
		return ;
	i = 0;
	while(split[i])
	{
		end = 0;
		if (split[i+1] == NULL)
			end = end_flag;//FIXME per posar 0 o 1correctaent
		cleaned = remove_quotes(split[i]);
		if (cleaned)

			add_token(new_list, type, cleaned, end);
		else
			add_token(new_list, type, ft_strdup(split[i]), end);
		i++;
	}
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++ ;
	}
	free(split);
/*
 *
** Divide el string `str` por espacios en blanco (puedes usar ft_split).
** Por cada palabra resultante, llama a add_token.
** El `end_flag` se pasa como 0 si estamos dentro de una expansión que divide, para que se comporten como tokens independientes.
*/

}

static void	exp_token_value(const char *t_val,t_token **n_lst, t_token *c, char *env[])
{
	//TODO Al llamar al add_token, no poner 1 en el end siempre, en el caso del
	//final, respetar el end de lo que ya teniamos
	int	quote; // 0= no quote, 1 = '', 2= ""
	int	i;
	char	*result;
	int	size;
	char	*token_exp;
	char	*cleaned;

	quote = ft_quoted_type(t_val[0]);
	i = 0;
	result = NULL;
	size = 0;
	while (t_val[i] != '\0')
	{
		if (t_val[i] == '$' && quote != 1)
		{
			//si hay parte literal antes de el $
			if (size > 0)
			{
				result = ft_realloc(result, size, size + 1);
				result[size] = '\0';
				cleaned = remove_quotes(result);
				if (!cleaned)
					cleaned = ft_strdup(result);
				free(result);
				add_token(n_lst,c->type, cleaned, 1); 
				result = NULL;
				size = 0;
			}
			if (t_val[i+1] == '?')
			{
				printf("gestionar $?");
				i += 2;
				//token_exp = expand_exit_status();
				//add_token(n_lst,current->type, token_exp, 1); 
			}
			else
			{
				token_exp = expand_variable_at(t_val, &i, env);
				if (ft_strchr(token_exp, ' ') && quote == 0)
					split_and_add_token(n_lst, c->type, token_exp, 1);
				else
				{
					cleaned = remove_quotes(token_exp);
					if(!cleaned)
						cleaned = ft_strdup(token_exp);
					free(token_exp);
					add_token(n_lst, c->type, cleaned, 1);
				}
			}
		}
		else
		{
			//TODO Modificar para que no incluya las comillas
			result = ft_realloc(result, size, size+1);
			result[size] = t_val[i];
			size ++;
			i ++;
		}
	}
	//si al final quedo algo, añadirlo como token
	if (size > 0)
	{
		result = ft_realloc(result, size, size + 1);
		result[size] = '\0';
		cleaned = remove_quotes(result);
		if (!cleaned)
			cleaned = ft_strdup(result);
		free(result);
		add_token(n_lst, c->type, cleaned, 1);
	}
}

void	process_expansion(t_token **tokens,  char *envp[])
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
			exp_token_value(current->value,&new_list, current, envp);
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
