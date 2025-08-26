/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:30:33 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/19 18:30:36 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_get_env_value(const char *key, t_list *env)
{
	t_env	*var;
	int		len;

	len = ft_strlen(key);
	while (env)
	{
		var = (t_env *)env->content;
		if (ft_strncmp(var->key, key, len) == 0)
			return (var->kval);
		env = env->next;
	}
	return (NULL);
}

void	ft_proc_files_redir_cmd(t_cmd *cmd)
{
	if (cmd->infile > 2)
	{
		dup2(cmd->infile, STDIN_FILENO);
		close(cmd->infile);
	}
	if (cmd->outfile > 2)
	{
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
	}
}

// function to inform if there is a redirect file error
int	ft_proc_files_redir_error(t_cmd *cmd, t_list *l_env)
{
	int	ret_val;

	ret_val = 1;
	if (cmd->infile == -1)
	{
		//close(STDIN_FILENO);
		ret_val = 0;
	}
	if (cmd->outfile == -1)
	{
		//close(STDOUT_FILENO);
		ret_val = 0;
	}
	if (ret_val == 0)
		ft_lstclear(&l_env, ft_free_env);
	return (ret_val);
}

// function to redirect input and output to the correspoding to process
//  it is taking into account if input is heredoc and also
// is managed signals in this fucntion due to norminette restrictions
void	ft_proc_pline_red(int **pipeline, int proc, int n_procs, t_cmd *cmd)
{
	ft_setup_signals(2);
	if (!pipeline || n_procs < 2)
		return ;
	if (proc > 0 && cmd->heredoc_count == 0)
		dup2(pipeline[proc - 1][0], STDIN_FILENO);
	if (proc < (n_procs - 1))
		dup2(pipeline[proc][1], STDOUT_FILENO);
	ft_close_pipes(pipeline, n_procs - 1);
}
