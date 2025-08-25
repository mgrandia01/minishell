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
static void	ft_parent_proc(int n_procs, pid_t pid)
{
	int	i;
	int	wstatus;

	i = 0;
	while (i < n_procs)
	{
		if (i == (n_procs - 1))
		{
			wstatus = 0;
			waitpid(pid, &wstatus, 0);
			if (WIFEXITED(wstatus))
				g_exit_status = WEXITSTATUS(wstatus);
			else
			{
				if (WIFSIGNALED(wstatus))
					g_exit_status = 128 + WTERMSIG(wstatus);
			}
		}
		i++;
	}
}

// function to inform if there is a redirect file error
static int	ft_proc_files_redir_error(t_cmd *cmd, t_list *l_env)
{
	int	ret_val;

	ret_val = 1;
	if (cmd->infile == -1)
	{
		close(STDIN_FILENO);
		ret_val = 0;
	}
	if (cmd->outfile == -1)
	{
		close(STDOUT_FILENO);
		ret_val = 0;
	}
	if (ret_val == 0)
		ft_lstclear(&l_env, ft_free_env);
	return (ret_val);
}

// function to redirect input and output to the correspoding to process
//  it is taking into account if input is heredoc and also
// is managed signals in this fucntion due to norminette restrictions
static void	ft_proc_pline_red(int **pipeline, int proc, int n_procs, t_cmd *cmd)
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

// function to create child processes
static int	ft_cr_procs(int n_procs, int **pipeline, t_cmd *cmd, t_list *l_env)
{
	int		i;
	pid_t	pid;
	t_cmd	*curr_cmd;

	curr_cmd = cmd;
	i = 0;
	while (i < n_procs && curr_cmd)
	{
		ft_manage_heredoc(curr_cmd, l_env);
		pid = fork();
		if (pid < 0)
			return (0);
		else if (pid == 0)
		{
			ft_proc_pline_red(pipeline, i, n_procs, cmd);
			if (ft_proc_files_redir_error(curr_cmd, l_env))
				ft_execute_process(curr_cmd, l_env);
			ft_child_erro_exit(cmd, pipeline);
		}
		curr_cmd = curr_cmd->next;
		i++;
	}
	ft_close_pipes(pipeline, n_procs - 1);
	ft_parent_proc(n_procs, pid);
	return (1);
}

// el caso de builtin sin pipes se ha de ejecutar fuera de la pipeline
// y de los hijos para conservar el entorno
// (los hijos pierden el entorno cuando finalizan)
// Ignorar SIGINT en el padre mientras se ejecutan comandos
// calcular n_pipes, crear array de pipes dinamicamente, crear procesos
// liberar array de pipes y restaurar senyales
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
