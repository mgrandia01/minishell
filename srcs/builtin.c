/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:31:40 by arcmarti          #+#    #+#             */
/*   Updated: 2025/07/19 11:31:43 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_builtin_cd(char **args)
{
	if (!args[1])
	{
		write(STDERR_FILENO, "minishell: cd: missing argument\n", 32);
		return (1);
	}
	if (chdir(args[1]) != 0)
	{
		write(STDERR_FILENO, "minishell: cd: ", 15);
		write(STDERR_FILENO, args[1], ft_strlen(args[1]));
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
		write(STDERR_FILENO, "\n", 1);
		return (1);
	}
	return (0);
}

int	ft_builtin_pwd(void)
{
	char	buffer[PATH_MAX];

	if (getcwd(buffer, sizeof(buffer)))
	{
		write(STDOUT_FILENO, buffer, ft_strlen(buffer));
		write(STDOUT_FILENO, "\n", 1);
	}
	else
	{
		write(STDERR_FILENO, "minishell: pwd: ", 16);
		write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
		return (1);
	}
	return (0);
}

int	ft_builtin_echo(char **args)
{
	int	i;
	int	add_cr;
	
	add_cr = 1;
	i = 1;
	if (args[i] && !ft_strncmp(args[i], "-n", 3))
	{
		add_cr = 0;
		i++;
	}
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (add_cr)
	{
			write(STDOUT_FILENO, "\n", 1);
	}
	return (0);
}

int	ft_builtin_env(char **args, char **envp)
{
	int	i;

	if (args[1])
	{
		ft_putstr_fd("minishell: env: '", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd("': No such file or directory\n", STDERR_FILENO);
		return (127);
	}
	i = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
		{
			ft_putstr_fd(envp[i], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		i++;
	}
	return (0);
}

int	ft_builtin_exit(char **args)
{
	ft_printf(STDOUT_FILENO, "exit\n");
	if (!args[1])
		exit (0);
	if (!ft_is_numeric(args[1]))
	{
		ft_printf(STDERR_FILENO, "minishell: exit: %s: numeric argument required\n", args[1]);
		exit (255);
	}
	if (args[2])
	{
		ft_printf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		return (1);
	}
	exit(ft_atoi(args[1]) % 256);
}

