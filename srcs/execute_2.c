/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 11:12:33 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/03 11:12:35 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_change_std_fds(t_cmd	*cmd)
{
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
}

void	ft_handle_single_builtin(t_cmd *cmd, t_list *l_env)
{
	int		status[2];

	status[0] = dup(STDIN_FILENO);
	status[1] = dup(STDOUT_FILENO);
	ft_change_std_fds(cmd);
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

void	ft_manage_parent_exit_status(int pid)
{
	int	wstatus;
	int	g_exit_status; // atencion, en el main esta como global, segun enunciado parece global

	wstatus = 0;
	g_exit_status = 0;
	waitpid(pid, &wstatus, 0);
	if (WIFEXITED(wstatus))
		g_exit_status = WEXITSTATUS(wstatus);
	else
	{
		if (WIFSIGNALED(wstatus))
			g_exit_status = 128 + WTERMSIG(wstatus);
	}
}

void	ft_parent_process(t_cmd *cmd, int *prev_fd, int pipefd[2], int pid)
{
	if (cmd->argv)
	{
		printf("\nINICIO PADRE------------------------%s--------------------INICIO PADRE", cmd->argv[0]);
		fflush(0);
	}
	else
	{
		printf("\nINICIO PADRE------------------------VACIO-----------------INICIO PADRE");
		fflush(0);
	}
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->infile > 2)
		close(cmd->infile);
	if (cmd->outfile > 2)
		close(cmd->outfile);
	if (cmd->next)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
	else
		*prev_fd = -1;
	ft_manage_parent_exit_status(pid);
	printf("\nFIN PADRE--------------------cmd->next: %p----------------------FIN PADRE\n", cmd);
	fflush(0);
}

static void	ft_command_not_found(char *path, t_cmd *cmd, char **envp_exec)
{
	if (!path)
	{
		if (cmd->argv)
			printf("minishell: %s: command not found\n", cmd->argv[0]);
		ft_free_tab(envp_exec);
		ft_cmdclear (&cmd, ft_free_argv);
		exit(127);
	}
}

void	ft_child_process_execute(t_cmd *cmd, t_list *l_env, int pipefd[2])
{
	int		exit_code;
	char	**envp_exec;
	char	*path;

	if (cmd->argv && ft_is_builtin(cmd->argv[0]))
	{
		exit_code = ft_execute_builtin(cmd->argv, l_env); // cuidado, yo creo que aqui l_env habra que cambiar a por redferencia pq se modicica !!!!!!
		ft_lstclear(&l_env, ft_free_env);
		ft_cmdclear (&cmd, ft_free_argv);
		/*if (cmd->infile > 2)
			close(cmd->infile);
		if (cmd->outfile > 2)
			close(cmd->outfile);
		if (prev_fd != -1)
			close(prev_fd);*/
		if (pipefd[1] != -1)
			close(pipefd[1]);
		exit(exit_code);
	}
	else
	{
		envp_exec = ft_build_envp_array(l_env);
		if (cmd->argv)
			path = find_path(cmd->argv[0], envp_exec);
		else
			path = NULL;
		ft_lstclear(&l_env, ft_free_env);
		ft_command_not_found(path, cmd, envp_exec);
		execve(path, cmd->argv, envp_exec);
		if (pipefd[1] != -1)
			close(pipefd[1]);
		ft_free_tab(envp_exec);
		free(path); // liberar la mem del main (input, token y cmd). provocar el fallo con /usr/bin echo "paco" (con el espacio)
	}
}


void	ft_child_process(t_cmd *cmd, t_list *l_env, int pipefd[2], int prev_fd)
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
	signal(SIGINT, SIG_DFL);	// Restablece comportamiento por defecto
	signal(SIGQUIT, SIG_DFL);	// Para Ctrl+\ también
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
		ft_printf(STDERR_FILENO, "minishell: %d: %s\n", cmd->outfile, strerror(errno));
		ft_cmdclear (&cmd, ft_free_argv);
		ft_lstclear(&l_env, ft_free_env);
		if (pipefd[0] != -1)
			close(pipefd[0]);
		if (pipefd[1] != -1)
			close(pipefd[1]);
		exit(1);
	}
	else if (cmd->outfile > 2)
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
	ft_child_process_execute(cmd, l_env, pipefd);
	ft_printf(STDERR_FILENO, "minishell: %s: %s\n", cmd->argv[0], strerror(errno));
	if (cmd->infile > 2)
		close(cmd->infile);
	if (cmd->outfile > 2)
		close(cmd->outfile);
	if (prev_fd != -1)
		close(prev_fd);
	if (pipefd[1] != -1)
		close(pipefd[1]);
	printf("\nFIN HIJO------(ha petado %s un execvce para haber llegado aqui----------------FIN HIJO", cmd->argv[0]);fflush(0);
	ft_cmdclear (&cmd, ft_free_argv);
	exit(127);
}

void	ft_manage_heredoc(t_cmd *cmd)
{
	if (cmd->heredoc == 1 && cmd->heredoc_delim)
	{
		cmd->infile = ft_create_heredoc(cmd->heredoc_delim);
		if (cmd->infile == -1)
		{
			ft_putstr_fd("minishell: error creando heredoc\n", STDERR_FILENO);
			return ;
		}
	}
}

void	ft_process_command(t_cmd *cmd, t_list *l_env, int *prev_fd, int pipefd[2])
{
	pid_t	pid;

	pid = 0;
	if (cmd->next && pipe(pipefd) == -1)
	{
		perror("Minishell: pipe");
		return ;	// estaba a break ;
	}
	ft_manage_heredoc(cmd);
	pid = fork();
	if (pid == -1)
	{
		perror("Minishell: fork");
		return ;	// estaba a break ;
	}
	if (pid == 0) // hijo
		ft_child_process(cmd, l_env, pipefd, *prev_fd);
	else // padre
		ft_parent_process(cmd, prev_fd, pipefd, pid);
}
