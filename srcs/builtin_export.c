/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 21:35:23 by arcmarti          #+#    #+#             */
/*   Updated: 2025/07/22 21:35:27 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_print_exp_vars(void *content)
{
	t_env   *var;

	var = (t_env *)content;
	if (var->kexp == 1)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(var->key, STDOUT_FILENO);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(var->kval, STDOUT_FILENO);
		ft_putstr_fd("\"\n", STDOUT_FILENO);
	}
}

static t_env	*ft_find_env_var(t_list *env_list, const char *key)
{
	t_env	*var;
	while (env_list)
	{
		var = (t_env *)env_list->content;
		if (ft_strncmp(var->key, key, ft_strlen(key) + 1) == 0)
			return (var);
		env_list = env_list->next;
	}
	return (NULL);
}

static int	ft_export_parse_key_value(char *arg, char **key, char **value)
{
	char    *equal;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		*key = ft_substr(arg, 0, equal - arg);
		*value = ft_strdup(equal + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
	if (!*key || (equal && !*value))
		return (1);
	return (0);
}

static void	ft_export_assign_var(char *key, char *value, t_list **l_env)
{
	t_env   *existing;
	t_env   *new;
	
	existing = ft_find_env_var(*l_env, key);
	if (existing)
	{
		if (value)
		{
			free(existing->kval);
			existing->kval = value;
		}
		existing->kexp = 1;
		free(key);
	}
	else
	{
        	new = (t_env *)malloc(sizeof(t_env));
	        if (!new)
        	    return ;
	        new->key = key;
		if (value)
			new->kval = value;
		else
			new->kval = ft_strdup("");
        	new->kexp = 1;
        	ft_lstadd_back(l_env, ft_lstnew(new));
    	}
}


int	ft_builtin_export(char **args, t_list *l_env)
{
	int	i;
	char	*key;
	char	*value;

	if (!args[1])
	{
        	ft_lstiter(l_env, ft_print_exp_vars);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (ft_export_parse_key_value(args[i], &key, &value))
			return (1);
		if (!ft_is_valid_key(key))
		{
			ft_putstr_fd("export: not a valid identifier\n", STDERR_FILENO);
			free(key);
			if (value)
				free(value);
		}
	        else
	               ft_export_assign_var(key, value, &l_env);
		i++;
	}
	return (0);
}
