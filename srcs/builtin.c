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
	char buffer[PATH_MAX];
	
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

