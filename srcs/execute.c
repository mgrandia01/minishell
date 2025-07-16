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
void	ft_exe_pipeline(t_cmd *cmd, char **envp);

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
	char *path;
	
	(void)envp;
	pid = fork();
	if (pid == 0)
	{
        	printf("\nHijo------------------------ cmds->argv[0] %s --------------------------", cmds->argv[0]);fflush(0);
		ft_printf("\n");
            	for(int j=0; j<5; j++)
            	{
			if (!cmds->argv[j])
				break ;
			ft_printf("Comando %d: %s\t", j, cmds->argv[j]);
		}
		ft_printf("\n");
		path = find_path(cmds->argv[0], envp);
            	printf("\nPath: %s\n", path);fflush(0);
            	for(int j=0; j<20; j++)
            	{
			if (!cmds->argv[j])
				break ;
			ft_printf("Argv[%d]: %s\t", j, cmds->argv[j]);
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


void	ft_exe_pipeline(t_cmd *cmd, char **envp)
{
	int     pipefd[2];
	int     prev_fd;
	int	status[2];
	pid_t   pid;
	char	*path;
	int	wstatus;
	int	g_exit_status;

	prev_fd = -1;
	pipefd[0] = -1;
	pipefd[1] = -1;
	if (!cmd)
		return ;
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
		ft_execute_builtin(cmd->argv);
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
			
			
			path = find_path(cmd->argv[0], envp);
			if (!path)
			{
				printf("minishell: %s: command not found\n", cmd->argv[0]);
				exit(127);
			}
			
			if (ft_is_builtin(cmd->argv[0]))
				ft_execute_builtin(cmd->argv);
			else
				execve(path, cmd->argv, envp);
			printf("minishell: %s: %s\n", cmd->argv[0], strerror(errno));
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
			printf("\nFIN PADRE--------------------cmd->next: %p----------------------FIN PADRE", cmd);fflush(0);
		}
	}
	ft_setup_signals();
}


