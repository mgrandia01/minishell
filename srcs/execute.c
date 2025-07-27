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



/*void	setup_signals(void)
{
    signal(SIGINT, handle_sigint);  // Ctrl+C
    signal(SIGQUIT, SIG_IGN);       // Ctrl+\
}*/

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
	if (!ft_strncmp(cmd, "cd", 3))
		return (1);
	if (!ft_strncmp(cmd, "echo", 5))
		return (1);
	if (!ft_strncmp(cmd, "pwd", 4))
		return (1);
	if (!ft_strncmp(cmd, "env", 4))
		return (1);
	if (!ft_strncmp(cmd, "exit", 5))
		return (1);
	if (!ft_strncmp(cmd, "export", 7))
		return (1);
	if (!ft_strncmp(cmd, "unset", 6))
		return (1);
	return (0);
}

int	ft_execute_builtin(char **cmd, t_list *l_env)
{
	if (!ft_strncmp(cmd[0], "pwd", 4))
        	return (ft_builtin_pwd());
	if (!ft_strncmp(cmd[0], "cd", 3))
		return (ft_builtin_cd(cmd));
	if (ft_strncmp(cmd[0], "echo", 5) == 0)
		return (ft_builtin_echo(cmd));
	if (ft_strncmp(cmd[0], "env", 4) == 0)
		return (ft_builtin_env(cmd, l_env));
	if (ft_strncmp(cmd[0], "exit", 5) == 0)
		return (ft_builtin_exit(cmd));
	if (ft_strncmp(cmd[0], "export", 7) == 0)
		return (ft_builtin_export(cmd, l_env));
	if (ft_strncmp(cmd[0], "unset", 6) == 0)
		return (ft_builtin_unset(cmd, &l_env));
	return 1; // error
}


void	ft_execute(t_cmd *cmds, char *envp[])
{
	pid_t pid;
	char *path;
	
	(void)envp;
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
}

static void sigint_handler_heredoc(int sig)
{
	(void) sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130); // Bash devuelve 130 en este caso
}

int	ft_create_heredoc(const char *delim)
{
	int	pipefd[2];
	char	*line;
	struct sigaction sa;
	
	sa.sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	
	if (pipe(pipefd) == -1)
		return (-1);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
        	line = get_next_line(STDIN_FILENO);
		if (!line)
			break;
		// Sacar el salto de línea si lo hay
	        if(ft_strchr(line, '\n'))
	        	*ft_strchr(line, '\n') = '\0';
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]); // cerramos escritura, mantenemos lectura
	return (pipefd[0]); // devolvemos el fd de lectura
}


void	ft_exe_pipeline(t_cmd *cmd, t_list *l_env)
{
	int     pipefd[2];
	int     prev_fd;
	int	status[2];
	pid_t   pid, last_pid;
	char	*path;
	int	wstatus;
	int	g_exit_status;
	char	**envp_exec;

	last_pid = -1;
	prev_fd = -1;
	pipefd[0] = -1;
	pipefd[1] = -1;
	envp_exec = NULL;
	//(void) g_exit_status;
	//(void) last_pid;
	if (!cmd)
		return ;
		
	pid = 0; path =NULL; wstatus=0, g_exit_status=0;
		
	// Ejecutar built-in directamente en el padre si es un único comando
	// Teoricamente ejeuta los builtin dentro de pipes pero pierde el resultado
	// asi que solo guardamos el estado si es el primer comando
	if (cmd->next == NULL && ft_is_builtin(cmd->argv[0]))
	{
		status[0] = dup(STDIN_FILENO);
		status[1] = dup(STDOUT_FILENO);
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
		if (ft_execute_builtin(cmd->argv, l_env))
		{
			//para ek futuro. PWD no hace falta
			//error
		}
		dup2(status[0], STDIN_FILENO);
		dup2(status[1], STDOUT_FILENO);
		close(status[0]);
		close(status[1]);
		return ;
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
		if (cmd->heredoc == 1 && cmd->heredoc_delim)
		{
			cmd->infile = ft_create_heredoc(cmd->heredoc_delim);
			if (cmd->infile == -1)
			{
				ft_putstr_fd("minishell: error creando heredoc\n", STDERR_FILENO);
				return ;
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("Minishell: fork");
			break ;
		}
		if (pid == 0) // hijo
		{
			printf("\nINICIO HIJO------Comando cmd->argv[0]: %s --------------------INICIO HIJO", cmd->argv[0]);fflush(0);
			
			signal(SIGINT, SIG_DFL);   // Restablece comportamiento por defecto
			signal(SIGQUIT, SIG_DFL);  // Para Ctrl+\ también
			printf("\nEXECUTE HIJO-----------------------");fflush(0);
			if (cmd->infile == -1)
			{
				    //ft_putstr_fd("minishell: error redirección de entrada\n", 2);
				    // el parser es el que tiene el noimbre del fichero
				    // minishell: inputfile.xxx: No such file or directory (lo coge de strerror(errno))
				    exit(EXIT_FAILURE); // Solo este proceso termina
			}else if (cmd->infile > 2)
			{
				dup2(cmd->infile, STDIN_FILENO);
				close(cmd->infile);
			}else if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			
			if (cmd->outfile == -1)
			{
				ft_putstr_fd("minishell: redirección de salida fallida\n", STDERR_FILENO);
				exit(1);
			}else if (cmd->outfile > 2)
			{
				dup2(cmd->outfile, STDOUT_FILENO);
				close(cmd->outfile);
			}else if (cmd->next)
			{
				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
			}
			
			if (pipefd[0] != -1)
               		 close(pipefd[0]);
			
			envp_exec = ft_build_envp_array(l_env);
			path = find_path(cmd->argv[0], envp_exec);
			if (!path)
			{
				printf("minishell: %s: command not found\n", cmd->argv[0]);
				ft_lstclear(&l_env, ft_free_env);
				ft_free_tab(envp_exec);
				ft_cmdclear (&cmd, ft_free_argv);
				exit(127);
			}
			
			if (ft_is_builtin(cmd->argv[0]))
			{
				exit((ft_execute_builtin(cmd->argv, l_env))); // ojo con liberar el envp_exec
			}
			else
			{	
				ft_lstclear(&l_env, ft_free_env);
				execve(path, cmd->argv, envp_exec);
			}
			ft_free_tab(envp_exec);
			//ft_printf("minishell: %s: %s\n", cmd->argv[0], strerror(errno)); //ft_printf no tiene STDERR
			ft_printf(STDERR_FILENO, "minishsssell: %s: %s\n", cmd->argv[0], strerror(errno));
			ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
			ft_putstr_fd(strerror(errno), STDERR_FILENO);
			ft_putstr_fd("\n", STDERR_FILENO);
			free(path); // liberar la mem del main (input, token y cmd). provocar el fallo con /usr/bin echo "paco" (con el espacio)
			if (cmd->infile > 2)
				close(cmd->infile);
			if (cmd->outfile > 2)
				close(cmd->outfile);
			if (prev_fd != -1)
				close(prev_fd);
			if (pipefd[1] != -1)
				close(pipefd[1]);
			printf("\nFIN HIJO------(ha petado un execvce para haber llegado aqui----------------FIN HIJO");fflush(0);
			ft_cmdclear (&cmd, ft_free_argv);
			exit(127);
		}
		else // padre
		{
			printf("\nINICIO PADRE------------------------%s--------------------INICIO PADRE", cmd->argv[0]);fflush(0);
			if (prev_fd != -1)
				close(prev_fd);
			if (cmd->infile > 2)
		                close(cmd->infile);
	                if (cmd->outfile > 2)
             		    close(cmd->outfile);
			if (cmd->next)
			{
				close(pipefd[1]);
				prev_fd = pipefd[0];
			}
			else
				prev_fd = -1;
			waitpid(pid, &wstatus, 0);
			if (WIFEXITED(wstatus))
				g_exit_status = WEXITSTATUS(wstatus);
			else if (WIFSIGNALED(wstatus))
				g_exit_status = 128 + WTERMSIG(wstatus);
			cmd = cmd->next;
			printf("\nFIN PADRE--------------------cmd->next: %p----------------------FIN PADRE\n", cmd);fflush(0);
		}
		
	}
	ft_setup_signals();
}


