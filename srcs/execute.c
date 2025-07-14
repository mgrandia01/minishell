/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 12:13:07 by arcmarti          #+#    #+#             */
/*   Updated: 2025/06/29 12:13:29 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_setup_signals(void);
void	ft_execute(t_cmd *cmds, char *envp[]);
int	ft_is_builtin(char *cmd);
int	ft_execute_builtin(char **cmd);


// en los forks de las ejeciones, si son hijos hay que restarurar el estado
       	// de las senales
       	//if (pid == 0)
       	//{
    	//  signal(SIGINT, SIG_DFL);
	//    signal(SIGQUIT, SIG_DFL);
	//    ...
	//

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
	if (!ft_strncmp(cmd, "cd", 2))
		return (1);
	if (!ft_strncmp(cmd, "echo", 4))
		return (1);
	if (!ft_strncmp(cmd, "pwd", 3))
		return (1);
	if (!ft_strncmp(cmd, "export", 6))
		return (1);
	if (!ft_strncmp(cmd, "unset", 5))
		return (1);
	if (!ft_strncmp(cmd, "env", 3))
		return (1);
	if (!ft_strncmp(cmd, "exit", 4))
		return (1);
	return (0);
}

int	ft_execute_builtin(char **cmd)
{
	(void) cmd;
	return 1;
}


void	ft_execute(t_cmd *cmds, char *envp[])
{
	pid_t pid;
	
	(void)envp;
	pid = fork();
	if (pid == 0)
	{
        	printf("\nHijo------------------------ %s %d--------------------------", cmds->argv[0], cmds->outfile);fflush(0);
		ft_printf("\n");
            	for(int j=0; j<5; j++)
            	{
			if (!cmds->argv[j])
				break ;
			ft_printf("Comando %d: %s\t", j, cmds->argv[j]);
		}
		ft_printf("\n");
		if (cmds->outfile >= 3)
		{
			dup2(cmds->outfile, STDOUT_FILENO);
            		close(cmds->outfile);
            	}
            	printf("\nlll %s llll", cmds->argv[1]);
            	if (execve(cmds->argv[0], cmds->argv, envp) == -1)
		{
			ft_printf("%s: %s %s \n", strerror(errno), cmds->argv[0], cmds->argv[1]); // valorar la posiblidad de customiziar ftprintf a la STDERR
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
}

void	ft_exe_pipeline(t_cmd *cmd, char **envp)
{
	int     pipefd[2];
	int     prev_fd = -1;
	int	status[2];
	pid_t   pid;

	if (!cmd || !ft_validate_fds(cmd))
		return ;
	// Ejecutar built-in directamente en el padre si es un único comando
	if (cmd->next == NULL && ft_is_builtin(cmd->argv[0]))
	{
		status[0] = dup(STDIN_FILENO);
		status[1] = dup(STDOUT_FILENO);
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
		ft_execute_builtin(cmd->argv);
		dup2(status[0], STDIN_FILENO);
		dup2(status[1], STDOUT_FILENO);
		close(status[0]);
		close(status[1]);
		return ;
	}
	// Ejecutar pipeline (comandos encadenados)
	while (cmd)
	{
		if (cmd->next && pipe(pipefd) == -1)
		{
			perror("Minishell: pipe");
			break ;
		}
		pid = fork();
		if (pid == -1)
		{
			perror("Minishell: fork");
			break ;
		}
		if (pid == 0) // hijo
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (cmd->next)
			{
				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
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
			if (ft_is_builtin(cmd->argv[0]))
				ft_execute_builtin(cmd->argv);
			else
				execve(cmd->argv[0], cmd->argv, envp);
			perror("Minishell: execve");
			exit(1);
		}
		else // padre
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (cmd->next)
			{
				close(pipefd[1]);
				prev_fd = pipefd[0];
			}
			waitpid(pid, NULL, 0);
			cmd = cmd->next;
		}
	}
}


