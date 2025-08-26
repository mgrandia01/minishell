/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:08:46 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/25 12:06:09 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_handle_execve_error(char *cmd)
{
	ft_printf(STDERR_FILENO, "minishell: %s: %s\n", cmd, strerror(errno));
	if (errno == ENOENT)
		g_exit_status = 127;
	else if (errno == EACCES)
		g_exit_status = 126;
	else
		g_exit_status = 1;
}

static int	ft_search_cmd(char *path, t_cmd *cmd)
{
	if (!path)
	{
		if (cmd && cmd->argv && cmd->argv[0])
		{
			ft_printf(STDERR_FILENO, "minishell: %s: ", cmd->argv[0]);
			ft_putstr_fd("command not found\n", STDERR_FILENO);
		}
		return (0);
	}
	return (1);
}

// function to exexute std command in child processes
static void	ft_execute_command(t_cmd *cmd, t_list *l_env)
{
	char	**envp_exec;
	char	*path;

	envp_exec = ft_build_envp_array(l_env);
	if (cmd->argv)
		path = find_path(cmd->argv[0], envp_exec);
	else
		path = NULL;
	ft_lstclear(&l_env, ft_free_env);
	if (ft_search_cmd(path, cmd))
	{
		ft_proc_files_redir_cmd(cmd);
		if (execve(path, cmd->argv, envp_exec) == -1)
			ft_handle_execve_error(cmd->argv[0]);
	}
	else
		g_exit_status = 127;
	ft_free_tab(envp_exec);
	free(path);
}

// function to execute builtin o std command in child processes
void	ft_execute_process(t_cmd *cmd, t_list *l_env)
{
	if (cmd->argv && ft_is_builtin(cmd->argv[0]))
	{
		ft_proc_files_redir_cmd(cmd);
		g_exit_status = ft_execute_builtin(cmd, l_env);
		ft_lstclear(&l_env, ft_free_env);
	}
	else
		ft_execute_command(cmd, l_env);
}

// function to free memory in case of error in child process
void	ft_child_erro_exit(t_cmd *cmd, int **pipeline, int *pid)
{
	ft_free_pipeline(pipeline);
	ft_cmdclear (&cmd, ft_free_argv);
	free(pid);
	exit (g_exit_status);
}
