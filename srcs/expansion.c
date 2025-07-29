/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/26 15:27:44 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	
	//TODO a que equival quote al inicio?
	//aixi elimina absolutament totes les comilles que troba
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

/* 
 * Searches for the value of an environment variable by name.
 * Returns a pointer to the value part (after the '=') if found in envp.
 * Returns NULL if the variable is not found.
 */
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
 * Expands an environment variable starting at index *i in t_val.
 * Looks up its value in envp and returns a duplicated string of the value.
 * Advances *i past the variable name.
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
