/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 12:13:07 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/22 09:11:46 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// function to manage parent where is waiting for children
static void	ft_parent_proc(int n_procs, pid_t *pid)
{
	int	i;
	int	wstatus;

	i = 0;
	while (i < n_procs)
	{
		wstatus = 0;
		//ft_setup_signals(1);
		waitpid(pid[i], &wstatus, 0);
		
		if (i == (n_procs - 1))
		{
			if (WIFEXITED(wstatus))
				g_exit_status = WEXITSTATUS(wstatus);
			else if (WIFSIGNALED(wstatus))
				g_exit_status = 128 + WTERMSIG(wstatus);
			else
				g_exit_status = 1;
		}
		i++;
	}
}

// function to free ft_cr_procs function variables
static void	ft_free_cr_procs(int **pipeline, int n_procs, int *pid)
{
	ft_close_pipes(pipeline, n_procs - 1);
	ft_parent_proc(n_procs, pid);
	free (pid);
}

// function to init ft_cr_procs function variables
static int	ft_init_cr_procs(t_cmd *cmd, t_cmd **curr, pid_t **pid, int n_procs)
{
	*curr = cmd;
	*pid = (pid_t *) malloc (sizeof(pid_t) * n_procs);
	if (!pid)
		return (0);
	else
		return (1);
}

// function to create child processes
static int	ft_cr_procs(int n_procs, int **pipeline, t_cmd *cmd, t_list *l_env)
{
	int		i;
	pid_t	*pid;
	t_cmd	*curr_cmd;

	if (!ft_init_cr_procs(cmd, &curr_cmd, &pid, n_procs))
		return (0);
	i = -1;
	while (++i < n_procs && curr_cmd)
	{
		if (!ft_manage_heredoc(curr_cmd, l_env))
			break ;
		pid[i] = fork();
		if (pid[i] < 0)
			return (0);
		else if (pid[i] == 0)
		{
			ft_proc_pline_red(pipeline, i, n_procs, curr_cmd);
			if (ft_proc_files_redir_error(curr_cmd, l_env))
				ft_execute_process(curr_cmd, l_env);
			ft_child_erro_exit(cmd, pipeline, pid);
		}
		curr_cmd = curr_cmd->next;
	}
	ft_free_cr_procs(pipeline, n_procs, pid);
	return (1);
}

// In case of alone builtin there is no pipeline execution, execution on parent
// execution on parent to keep the environment (childs lost env. when finish)
// Ignoring SIGINT on parent during command execution on childs
// Numbers of pipes calculation, dinamic pipeline array creation, 
// child processes creation, free pipeline and signals restore
void	ft_exe_pipeline(t_cmd *cmd, t_list *l_env)
{
	int	n_pipes;
	int	**pipeline;

	if (!cmd)
		return ;
	if (!cmd->next && cmd->argv && ft_is_builtin(cmd->argv[0]))
	{
		ft_manage_builtin_alone(cmd, l_env);
		return ;
	}
	ft_setup_signals(0);
	n_pipes = ft_calculate_pipes(cmd);
	if (!ft_create_pipes(&pipeline, n_pipes))
		ft_free_mem_exe(cmd, l_env, pipeline, 1);
	if (!ft_cr_procs(n_pipes + 1, pipeline, cmd, l_env))
		ft_free_mem_exe(cmd, l_env, pipeline, 1);
	ft_free_pipeline(pipeline);
	ft_setup_signals(1);
}
