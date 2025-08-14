/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 11:31:40 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/12 10:24:01 by mgrandia         ###   ########.fr       */
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

static int	ft_echo_is_n(char *arg)
{
	int i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
    	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_builtin_echo(char **args)
{
	int	i;
	int	add_cr;

	add_cr = 1;
	i = 1;
	if (args[i] && ft_echo_is_n(args[i]))
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

int	ft_builtin_env(char **args, t_list *l_env)
{
	t_env	*var;

	if (args[1])
	{
		ft_putstr_fd("minishell: env: '", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd("': No such file or directory\n", STDERR_FILENO);
		return (127);
	}
	while (l_env)
	{
		var = (t_env *) l_env->content;
		if (var->kexp && var->key && var->kval)
		{
			ft_putstr_fd(var->key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(var->kval, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		l_env = l_env->next;
	}
	return (0);
}

int	ft_builtin_exit(t_cmd *cmds, t_list *l_env)
{
	char	**args;
	int		arg1;

	args = cmds->argv;
	ft_lstclear(&l_env, ft_free_env);
	ft_printf(STDOUT_FILENO, "exit\n");
	if (!args[1])
	{
		ft_cmdclear (&cmds, ft_free_argv);
		g_exit_status = 0;
		return (0);
	}
	if (!ft_is_numeric(args[1]))
	{
		ft_cmdclear (&cmds, ft_free_argv);
		ft_printf(STDERR_FILENO, "minishell: exit: ");
		ft_printf(STDERR_FILENO, "%s: numeric argument required\n", args[1]);
		g_exit_status = 255;
		return (255);
	}
	if (args[2])
	{
		ft_cmdclear (&cmds, ft_free_argv);
		ft_printf(STDERR_FILENO, "minishell: exit: too many arguments\n");
		g_exit_status = 1;
		return (1);
	}
	arg1 = ft_atoi(args[1]) % 256;
	ft_cmdclear (&cmds, ft_free_argv);
	g_exit_status = arg1;
	return (arg1);
}
