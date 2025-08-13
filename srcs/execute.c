/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 12:13:07 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/12 10:36:11 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_validate_fds(t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->infile == -1 || cmd->outfile == -1)
			return (0);
		cmd = cmd->next;
	}
	return (1);
}

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

int	ft_execute_builtin(t_cmd *cmd, t_list *l_env)
{
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (ft_builtin_pwd());
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (ft_builtin_cd(cmd->argv));
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
	return (1); // error
}

/*void	ft_execute(t_cmd *cmds, char *envp[])
{
	pid_t pid;
	char *path;

	(void) envp;
	pid = fork();
	if (pid == 0)
	{
        	printf("\nHijo------------------------ cmds->argv[0] %s --------------------------", cmds->argv[0]);fflush(0);
		ft_printf(STDOUT_FILENO,"\n");
            	for(int j=0; j<5; j++)
            	{
			if (!cmds->argv[j])
				break ;
			ft_printf(STDOUT_FILENO, "Comando %d: %s\t", j, cmds->argv[j]);
		}
		ft_printf(STDOUT_FILENO, "\n");
		path = find_path(cmds->argv[0], envp);
            	printf("\nPath: %s\n", path);fflush(0);
            	for(int j=0; j<20; j++)
            	{
			if (!cmds->argv[j])
				break ;
			ft_printf(STDOUT_FILENO, "Argv[%d]: %s\t", j, cmds->argv[j]);
		}
		printf("\nInicio ejecucion antes desviar STDOUT\n");fflush(0);
		if (cmds->outfile >= 3)
		{
			dup2(cmds->outfile, STDOUT_FILENO);
            		close(cmds->outfile);
            	}
            	if (execve(path, cmds->argv, envp) == -1)
		{
			free(path);
			ft_printf(STDERR_FILENO,"%s: %s %s \n", strerror(errno), cmds->argv[0], cmds->argv[1]); 
			for(int j=0; j<5; j++)
			{
				if (!cmds->argv[j])
					break ;
				free (cmds->argv[j]);
			}
			free (cmds->argv);
			free (cmds);
			exit(1);
		}
		exit(0);
				
	}
	else
	{
        	waitpid(pid, NULL, 0);
        	printf("\nPadre------------------------ %s --------------------------", cmds->argv[0]);fflush(0);
        	// bug en ft_printf, no interpreta bien la "/" si forma parte del %s y mete un salto de linea
        	// separar comandos por pipe, no >>. se ve en este ejemplo
        }
}*/

void	ft_exe_pipeline(t_cmd *cmd, t_list *l_env)
{
	int		pipefd[2];
	int		prev_fd;
	t_cmd	*cmd_temp;

	//cmd_temp = NULL;
	prev_fd = -1;
	pipefd[0] = -1;
	pipefd[1] = -1;
	if (!cmd)
		return ;
	// Ejecutar built-in directamente en el padre si es un único comando
	// Teoricamente (bash???) ejecuta los builtin dentro de pipes pero pierde 
	// el resultado
	// asi que solo guardamos el estado si es el primer comando
	if (cmd->next == NULL && cmd->argv && ft_is_builtin(cmd->argv[0]))
	{
		ft_handle_single_builtin(cmd, l_env);
		ft_cmddelone(cmd, ft_free_argv);
		cmd = NULL;
	}
	// Ignorar SIGINT en el padre mientras se ejecutan comandos
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	// Ejecutar pipeline
	while (cmd)
	{
		ft_process_command(cmd, l_env, &prev_fd, pipefd);
		cmd_temp = cmd->next;
		ft_cmddelone(cmd, ft_free_argv);
		cmd = cmd_temp;
	}
	ft_setup_signals();
}
