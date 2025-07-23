/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/14 16:28:48 by mgrandia         ###   ########.fr       */
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


static char	*expand_variable_at(const char *token_valeu, int *i, char *envp[])
{
	// TODO las variables creadas por el usuario

/*
** A partir de la posición `*i`, detecta y extrae el nombre de la variable. alfanumerico o _
** Busca el valor correspondiente en `envp`.
** Devuelve el valor como string (puede contener espacios).
** Avanza el índice `*i` hasta el final del nombre de la variable para que el bucle lo continúe bien.
*/
}

static char	*expand_exit_status(vois)
{
/*
** Devuelve una string que representa el valor de la última variable $? expandida.
** Este valor debe estar guardado globalmente o ser accesible de alguna forma.
*/
}

static void	split_and_add_token(t_token **new_list, t_token_type type, char *token_exp, int end_flag)
{
/*
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
				add_token(new_list,current->type, result, 1); 
				result = NULL;
				size = 0;
			}
			if (t_val[i+1] == '?')
			{
				token_exp = expand_exit_status();
				add_token(new_list,current->type, token_exp, 1); 
			}
			else
			{
				token_exp = expand_variable_at(token_value, &i, env);
				if (ft_strchr(token_exp, ' ') && quote == 0)
					split_and_add_token(new_list, current->type, token_exp, 1);
				else
					add_token(new_list, current->type, token_exp, 1);
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
		add_token(new_list, current->type, result, 1);
	}
}

void	process_expansion(t_token **tokens,  char *envp[])
{
	t_token	*new_list;
	t_token	*current;
	char	*expanded;

	new_list = NULL;
	current = *tokens;
	(void)envp;
	while (current != NULL)
	{
		if (ft_strchr(current->value, '$'))
		{
			exp_token_value(current->value,&new_list, current, envp);
			//add_token(&new_list, current->type, expanded, current->end);
		}
		else
			//TODO Gestionar para que no incluya las comillas
			add_token(&new_list, current->type, ft_strdup(current->value), current->end);
		current = current->next;
	}
	free_token(*tokens);
	*tokens = new_list;
}
