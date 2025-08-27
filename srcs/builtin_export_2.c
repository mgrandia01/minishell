/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:31:29 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/02 10:39:16 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static void	ft_check_exists(t_env *current, char *key, char *value)
{
	if (value)
	{
		free(current->kval);
		current->kval = value;
	}
	current->kexp = 1;
	free(key);
}

void	ft_export_assign_var(char *key, char *value, t_list **l_env)
{
	t_env	*existing;
	t_env	*new;

	//printf("\nkey %s value %s", key, value);fflush(0);
	existing = ft_find_env_var(*l_env, key);
	if (existing)
		ft_check_exists(existing, key, value);
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
