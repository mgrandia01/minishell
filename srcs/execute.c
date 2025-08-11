/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 12:13:07 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/11 10:08:41 by mgrandia         ###   ########.fr       */
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

int	ft_execute_builtin(char **cmd, t_list *l_env)
{
	if (ft_strcmp(cmd[0], "pwd") == 0)
		return (ft_builtin_pwd());
	if (ft_strcmp(cmd[0], "cd") == 0)
		return (ft_builtin_cd(cmd));
	if (ft_strcmp(cmd[0], "echo") == 0)
		return (ft_builtin_echo(cmd));
	if (ft_strcmp(cmd[0], "env") == 0)
		return (ft_builtin_env(cmd, l_env));
	if (ft_strcmp(cmd[0], "exit") == 0)
		return (ft_builtin_exit(cmd));
	if (ft_strcmp(cmd[0], "export") == 0)
		return (ft_builtin_export(cmd, l_env));
	if (ft_strcmp(cmd[0], "unset") == 0)
		return (ft_builtin_unset(cmd, &l_env));
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
	pid_t	pid;
	pid_t	last_pid;

	(void)g_exit_status;
	(void)last_pid;
	last_pid = -1;
	prev_fd = -1;
	pipefd[0] = -1;
	pipefd[1] = -1;
	if (!cmd)
		return ;	
	pid = 0;
		
	// Ejecutar built-in directamente en el padre si es un único comando
	// Teoricamente (bash???) ejecuta los builtin dentro de pipes pero pierde el resultado
	// asi que solo guardamos el estado si es el primer comando
	if (cmd->next == NULL && cmd->argv && ft_is_builtin(cmd->argv[0]))
	{
		ft_handle_single_builtin(cmd, l_env);
		cmd = cmd->next; // si se ejecuta aqui que no entre a while para que no lo haga el hijo)
	}
	// Ignorar SIGINT en el padre mientras se ejecutan comandos
	
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	// Ejecutar pipeline
	while (cmd)
	{
		if (cmd->next && pipe(pipefd) == -1)
		{
			perror("Minishell: pipe");
			break ;
		}
		/*if (cmd->heredoc == 1 && cmd->heredoc_delim)
		{
			cmd->infile = ft_create_heredoc(cmd->heredoc_delim);
			if (cmd->infile == -1)
			{
				ft_putstr_fd("minishell: error creando heredoc\n", STDERR_FILENO);
				return ;
			}
		}
		
		if (cmd->infile == -1 || cmd->outfile == -1)
		{
			//ft_putstr_fd("minishell: error redirección de entrada\n", 2);
			// el parser es el que tiene el noimbre del fichero
			// minishell: inputfile.xxx: No such file or directory (lo coge de strerror(errno))
			ft_printf(STDERR_FILENO, "minishell: %d: %s\n", cmd->infile, strerror(errno));
			
			if (pipefd[0] != -1)
				close(pipefd[0]);
			if (pipefd[1] != -1)
				close(pipefd[1]);
			prev_fd = -1;
			
			cmd = cmd->next;
			continue ;
		}
		}*/
		pid = fork();
		if (pid == -1)
		{
			perror("Minishell: fork");
			break ;
		}
		if (pid == 0) // hijo
		{
			if (cmd->argv)
			{
				printf("\nINICIO HIJO------Comando cmd->argv[0]: %s --------------------INICIO HIJO", cmd->argv[0]);
				fflush(0);
			}
			else
			{
				printf("\nINICIO HIJO------Comando cmd->argv[0]: VACIO --------------------INICIO HIJO");
				fflush(0);
			}
			// si son hijos hay que restarurar el estado de la senyales
			signal(SIGINT, SIG_DFL);   // Restablece comportamiento por defecto
			signal(SIGQUIT, SIG_DFL);  // Para Ctrl+\ también
			printf("\nEXECUTE HIJO-----------------------");fflush(0);
			
			if (cmd->infile == -1)
			{
			//ft_putstr_fd("minishell: error redirección de entrada\n", 2);
			// el parser es el que tiene el noimbre del fichero
			// minishell: inputfile.xxx: No such file or directory (lo coge de strerror(errno))
				ft_printf(STDERR_FILENO, "minishell: %d: %s\n", cmd->infile, strerror(errno));
				ft_lstclear(&l_env, ft_free_env);
				ft_cmdclear (&cmd, ft_free_argv);
				if (pipefd[0] != -1)
					close(pipefd[0]);
				if (pipefd[1] != -1)
					close(pipefd[1]);
				exit(EXIT_FAILURE);
			}
			else if (cmd->infile > 2)
			{
				dup2(cmd->infile, STDIN_FILENO);
				close(cmd->infile);
			}
			else if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			
			if (cmd->outfile == -1)
			{
				ft_printf(STDERR_FILENO, "minishell: %d: %s\n", cmd->infile, strerror(errno));
				ft_cmdclear (&cmd, ft_free_argv);
				ft_lstclear(&l_env, ft_free_env);
				if (pipefd[0] != -1)
					close(pipefd[0]);
				if (pipefd[1] != -1)
					close(pipefd[1]);
				exit(1); 
			
			}else if (cmd->outfile > 2)
			{
				dup2(cmd->outfile, STDOUT_FILENO);
				close(cmd->outfile);
			}
			else if (cmd->next)
			{
				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
			}
			
			if (pipefd[0] != -1)
				close(pipefd[0]);
			
			ft_child_process(cmd, l_env);
			ft_printf(STDERR_FILENO, "minishell: %s: %s\n", cmd->argv[0], strerror(errno));
			if (cmd->infile > 2)
				close(cmd->infile);
			if (cmd->outfile > 2)
				close(cmd->outfile);
			if (prev_fd != -1)
				close(prev_fd);
			if (pipefd[1] != -1)
				close(pipefd[1]);
			printf("\nFIN HIJO------(ha petado un execvce para haber llegado aqui----------------FIN HIJO");
			fflush(0);
			ft_cmdclear (&cmd, ft_free_argv);
			exit(127);
		}
		else // padre
		{
			ft_parent_process(cmd, &prev_fd, pipefd, pid);
			cmd = cmd->next;
		}
	}
	ft_setup_signals();
}


