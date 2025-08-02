/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:31:29 by arcmarti          #+#    #+#             */
/*   Updated: 2025/07/27 15:31:31 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_print_error(char *str)
{
	write(STDERR_FILENO, "unset: '", 8);
	write(STDERR_FILENO, str, ft_strlen(str));
	write(STDERR_FILENO, "': not a valid identifier\n", 26);
}

static void	ft_remove_env(char *arg, t_list **l_env)
{
	t_env	*env;
	t_list	*curr;
	t_list	*prev;

	curr = *l_env;
	prev = NULL;
	while (curr)
	{
		env = (t_env *)curr->content;
		if (env && ft_strcmp(env->key, arg) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*l_env = curr->next;
			free(env->key);
			free(env->kval);
			free(env);
			free(curr);
			break ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	ft_builtin_unset(char **args, t_list **l_env)
{
	int		i;
	int		status;

	i = 1;
	status = 0;
	while (args[i])
	{
		if (!ft_is_valid_key(args[i]))
		{
			ft_print_error(args[i]);
			status = 1;
			i++;
			continue ;
		}
		ft_remove_env(args[i], l_env);
		i++;
	}
	return (status);
}
