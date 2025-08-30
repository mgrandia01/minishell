/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 15:31:17 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/30 17:14:02 by mgrandia         ###   ########.fr       */
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

// function to free structure
// heredoc free is done in another function where managing heredoc
void	ft_cmdclear(t_cmd **lst, void (*del)(char **))
{
	t_cmd	*ptr_next;
	int		i;

	if (lst && del)
	{
		while (*lst)
		{
			ptr_next = (*lst)->next;
			del((*lst)->argv);
			if ((*lst)->infile > 2)
				close((*lst)->infile);
			if ((*lst)->outfile > 2)
				close((*lst)->outfile);
			i = -1;
			if ((*lst)->outfile_count > 0)
			{
				while (++i < (*lst)->outfile_count)
					free((*lst)->outfile_name[i]);
				free((*lst)->outfile_name);
			}
			free(*lst);
			*lst = ptr_next;
		}
	}
}

void	ft_cmddelone(t_cmd *lst, void (*del)(char **))
{
	del((lst)->argv);
	ft_free_heredoc(lst);
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
