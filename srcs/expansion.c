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

static void	init_vars_to0(int *i, int*j, char *quote)
{
	*i = 0;
	*j = 0;
	*quote = 0;
}

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	quote;
	char	*result;

	init_vars_to0(&i, &j, &quote);
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
			continue ;
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

static char	*get_env_value(const char *name, char *envp[])
{
	int		i;
	size_t	len;

	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i ++;
	}
	return (NULL);
}

// TODO las variables creadas por el usuario

/*
** A partir de la posición `*i`, detecta y extrae el nombre de la variable.
alfanumerico o _
** Busca el valor correspondiente en `envp`.
** Devuelve el valor como string (puede contener espacios).
** Avanza el índice `*i` hasta el final del nombre de la variable para
que el bucle lo continúe bien.
*/
char	*expand_variable_at(const char *t_val, int *i, char *envp[])
{
	char	*var_name;
	char	*value;
	int		start;
	int		len;

	start = *i;
	(*i)++;
	len = 0;
	while (t_val[*i + len] && (ft_isalnum(t_val[*i + len])
			|| t_val[*i + len] == '_'))
		len++;
	var_name = ft_substr(t_val, start + 1, len);
	*i += len;
	if (!var_name)
		return (NULL);
	value = get_env_value(var_name, envp);
	free(var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}
