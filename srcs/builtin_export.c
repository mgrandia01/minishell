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
	t_env	*var;

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

static void	ft_free_invalid_key(char *key, char *value)
{
	ft_putstr_fd("export: not a valid identifier\n", STDERR_FILENO);
	free(key);
	if (value)
		free(value);
}

static int	ft_export_parse_key_value(char *arg, char **key, char **value)
{
	char	*equal;

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

int	ft_builtin_export(char **args, t_list *l_env)
{
	int		i;
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
			ft_free_invalid_key(key, value);
		}
		else
			ft_export_assign_var(key, value, &l_env);
		i++;
	}
	return (0);
}
