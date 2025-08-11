/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:31:17 by arcmarti          #+#    #+#             */
/*   Updated: 2025/07/27 15:31:21 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_env(void *content)
{
	t_env	*var;

	var = (t_env *)content;
	if (var)
	{
		free(var->key);
		free(var->kval);
		free(var);
	}
}

t_list	*ft_init_env(char **envp)
{
	t_list	*env_list;
	t_env	*var;
	int		i;
	char	*equal;

	env_list = NULL;
	i = -1;
	while (envp[++i])
	{
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			var = (t_env *)malloc(sizeof(t_env));
			if (!var)
			{
				ft_lstclear(&env_list, ft_free_env);
				return (NULL);
			}
			var->key = ft_substr(envp[i], 0, equal - envp[i]);
			var->kval = ft_strdup(equal + 1);
			var->kexp = 1;
			ft_lstadd_back(&env_list, ft_lstnew(var));
		}
	}
	return (env_list);
}

void	ft_free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	ft_cmdclear(t_cmd **lst, void (*del)(char **))
{
	t_cmd	*ptr_next;

	if (lst && del)
	{
		while (*lst)
		{
			ptr_next = (*lst)->next;
			del((*lst)->argv);
			//if ((*lst)->heredoc_delim)
			//	free((*lst)->heredoc_delim);
			/*if ((*lst)->heredoc_delim_1)
				free((*lst)->heredoc_delim_1);
			if ((*lst)->heredoc_delim_2)
				free((*lst)->heredoc_delim_2);*/
			free(*lst);
			*lst = ptr_next;
		}
	}
}

void	ft_cmddelone(t_cmd *lst, void (*del)(char **))
{
	del((lst)->argv);
	//if ((lst)->heredoc_delim)
	//	free((lst)->heredoc_delim);
	/*if ((*lst)->heredoc_delim_1)
		free((*lst)->heredoc_delim_1);
	if ((*lst)->heredoc_delim_2)
		free((*lst)->heredoc_delim_2);*/
	free (lst);
}

void	ft_free_argv(char **ptr)
{
	int	i;

	i = 0;
	if (ptr)
	{
		while (ptr[i])
		{
			free(ptr[i]);
			i++;
		}
		free(ptr);
	}
}
