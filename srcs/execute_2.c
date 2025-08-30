/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:12:33 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/30 11:50:32 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_change_std_fds(t_cmd	*cmd)
{
	if (cmd->infile == -1 || cmd->outfile == -1)
	{
		ft_putstr_fd("minishell: error en redirección\n", STDERR_FILENO);
		return ;
	}
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

void	ft_handle_single_builtin(t_cmd *cmd, t_list *l_env)
{
	int		status[2];
	int		is_exit;

	is_exit = 0;
	status[0] = dup(STDIN_FILENO);
	status[1] = dup(STDOUT_FILENO);
	ft_change_std_fds(cmd);
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		is_exit = 1;
	g_exit_status = ft_execute_builtin(cmd, l_env);
	dup2(status[0], STDIN_FILENO);
	dup2(status[1], STDOUT_FILENO);
	close(status[0]);
	close(status[1]);
	if (is_exit == 1)
		exit(g_exit_status);
	return ;
}

/*void	ft_command_not_found(char *path, t_cmd *cmd, char **envp_exec)
{
	if (!path)
	{
		if (cmd->argv)
			printf("minishell: %s: command not found\n", cmd->argv[0]);
		ft_free_tab(envp_exec);
		ft_cmdclear (&cmd, ft_free_argv);
		exit(127);
	}
}*/

/*void	ft_manage_heredoc(t_cmd *cmd, t_list *l_env)
{
	if (cmd->heredoc_count > 0 && cmd->heredocs)
	{
		cmd->infile = ft_cr_hdoc(cmd->heredocs, cmd->heredoc_count, cmd, l_env);
		if (cmd->infile == -1)
		{
			ft_free_heredoc(cmd);
			return ;
		}
	}
	ft_free_heredoc(cmd);
}*/

int	ft_manage_heredoc(t_cmd *cmd, t_list *l_env)
{
	if (cmd->heredoc_count > 0 && cmd->heredocs)
	{
		cmd->infile = ft_cr_hdoc(cmd->heredocs, cmd->heredoc_count, cmd, l_env);
		ft_free_heredoc(cmd);
		if (cmd->infile == -1)
			return (0);
	}
	return (1);
}

void	manage_level(char *path, t_list *l_env, char ***envp_exec)
{
	char	*c_shell;
	char	*a_shell;
	int		i_shell;

	a_shell = NULL;
	if (path && ft_strncmp(path, "./minishell", 11) == 0)
	{
		c_shell = ft_get_env_value("SHLVL", l_env);
		if (c_shell)
		{
			i_shell = ft_atoi(c_shell) + 1;
			a_shell = ft_itoa(i_shell);
		}
		if (a_shell)
		{
			ft_export_assign_var(ft_strdup("SHLVL"), a_shell, &l_env);
			ft_free_tab(*envp_exec);
			*envp_exec = ft_build_envp_array(l_env);
		}
	}
}
