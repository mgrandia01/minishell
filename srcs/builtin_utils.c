/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:07:49 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/25 10:46:44 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// function to call to execute a builtin
int	ft_execute_builtin(t_cmd *cmd, t_list *l_env)
{
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (ft_builtin_pwd(l_env));
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (ft_builtin_cd(cmd->argv, l_env));
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (ft_builtin_echo(cmd->argv));
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (ft_builtin_env(cmd->argv, l_env));
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (ft_builtin_exit(cmd, l_env));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (ft_builtin_export(cmd->argv, l_env));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (ft_builtin_unset(cmd->argv, &l_env));
	return (1);
}

// function to manage execution when only there is a builtin without pipeline
void	ft_manage_builtin_alone(t_cmd *cmd, t_list *l_env)
{
	int	parent_stdout;
	int	parent_stdin;

	parent_stdout = 0;
	parent_stdin = 0;
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		g_exit_status = ft_execute_builtin(cmd, l_env);
	parent_stdin = dup(STDIN_FILENO);
	parent_stdout = dup(STDOUT_FILENO);
	ft_manage_heredoc(cmd, l_env);
	ft_proc_files_redir_cmd(cmd);
	g_exit_status = ft_execute_builtin(cmd, l_env);
	dup2(parent_stdin, STDIN_FILENO);
	dup2(parent_stdout, STDOUT_FILENO);
	close(parent_stdin);
	close(parent_stdout);
}

// function to check if command is a builtin
int	ft_is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}

// function to move to home directory on builtin cd
int	ft_cd_ret_home(char **args, t_list *l_env)
{
	char	*home;
	char	*pwd;

	if (!args[1] || ft_strncmp(args[1], "~", 1) == 0)
	{
		home = ft_get_env_value("HOME", l_env);
		pwd = ft_get_env_value("PWD", l_env);
		if (!home)
		{
			ft_printf(STDERR_FILENO, "minishell: cd: %s: ", home);
			ft_printf(STDERR_FILENO, "%s\n", strerror(errno));
			return (0);
		}
		if (chdir(home) != 0)
		{
			ft_printf(STDERR_FILENO, "minishell: cd: %s: ", home);
			ft_printf(STDERR_FILENO, "%s\n", strerror(errno));
			return (0);
		}
		if (pwd)
			ft_export_assign_var(ft_strdup("OLDPWD"), ft_strdup(pwd), &l_env);
		ft_export_assign_var(ft_strdup("PWD"), ft_strdup(home), &l_env);
		return (1);
	}
	return (0);
}
