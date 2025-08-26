/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:52:24 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/19 12:52:27 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_cd_newcwd_error(void)
{
	write(STDERR_FILENO, "minishell: cd: ", 15);
	write(STDERR_FILENO, "error getting new directory: ", 29);
	ft_printf(STDERR_FILENO, "%s\n", strerror(errno));
	return (1);
}

static void	ft_cd_chdir_error(char *path, char **oldpwd)
{
	write(STDERR_FILENO, "minishell: cd: ", 15);
	ft_printf(STDERR_FILENO, "%s: %s\n", path, strerror(errno));
	free(*oldpwd);
}

static int	ft_cd_oldpwd_error(t_list *l_env)
{
	char	*home;

	write(STDERR_FILENO, "minishell: cd: ", 15);
	write(STDERR_FILENO, "error getting current directory: ", 33);
	ft_printf(STDERR_FILENO, "%s\n", strerror(errno));
	home = ft_get_env_value("HOME", l_env);
	if (!home)
	{
		write(STDERR_FILENO, "minishell: cd: HOME not set\n", 29);
		return (1);
	}
	write(STDERR_FILENO, "minishell: cd: Back to HOME\n", 28);
	if (chdir(home) != 0)
	{
		ft_printf(STDERR_FILENO, "minishell: cd: %s: ", home);
		ft_printf(STDERR_FILENO, "%s\n", strerror(errno));
		return (1);
	}
	ft_export_assign_var(ft_strdup("PWD"), ft_strdup(home), &l_env);
	return (1);
}

static int	ft_manage_cd_arguments(char **args)
{
	if (!args || !args[1])
	{
		write(STDERR_FILENO, "minishell: cd: missing argument\n", 32);
		return (0);
	}
	if (args[2])
	{
		write(STDERR_FILENO, "minishell: cd: too many arguments\n", 34);
		return (0);
	}
	return (1);
}

// ft_builtin_cd updates the current folder. Also OLDPWD and PWD env variables
// ft_export_assign_var is managing free of local variable path
/*int	ft_builtin_cd(char **args, t_list *l_env)
{
	char	*path;

	if (!ft_manage_cd_arguments(args))
		return (1);
	path = getcwd(NULL, 0);
	if (!path)
		return (ft_cd_oldpwd_error(l_env));
	if (chdir(args[1]) != 0)
	{
		ft_cd_chdir_error(args[1], &path);
		return (1);
	}
	ft_export_assign_var(ft_strdup("OLDPWD"), path, &l_env);
	path = getcwd(NULL, 0);
	if (!path)
		return (ft_cd_newcwd_error());
	ft_export_assign_var(ft_strdup("PWD"), path, &l_env);
	return (0);
}*/

// ft_builtin_cd updates the current folder. Also OLDPWD and PWD env variables
// ft_export_assign_var is managing free of local variable path
int	ft_builtin_cd(char **args, t_list *l_env)
{
	char	*path;
	char	*pwd;

	if (!ft_manage_cd_arguments(args))
		return (1);
	path = getcwd(NULL, 0);
	if (!path)
		return (ft_cd_oldpwd_error(l_env));
	if (chdir(args[1]) != 0)
	{
		ft_cd_chdir_error(args[1], &path);
		return (1);
	}
	pwd = ft_get_env_value("PWD", l_env);
	/*if (!pwd)
		ft_builtin_unset("PWD", &l_env);
		ft_builtin_unset("OLDPWD", &l_env);
	else
	{*/
	ft_export_assign_var(ft_strdup("OLDPWD"), path, &l_env);
	path = getcwd(NULL, 0);
	if (!path)
		return (ft_cd_newcwd_error());
	ft_export_assign_var(ft_strdup("PWD"), path, &l_env);
	//}
	return (0);
}
