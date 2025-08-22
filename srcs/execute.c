/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 12:13:07 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/22 09:11:46 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*void	ft_proc_pipeline_redir(int **pipeline, int proc, int n_proc)
{
	if (n_proc < 2)
		return ;
	if (proc == 0)
	{
		close(pipeline[0][0]);
		dup2(pipeline[0][1], STDOUT_FILENO);
		close(pipeline[0][1]);
		close(pipeline[1][0]);
		close(pipeline[1][1]);
		close(pipeline[2][0]);
		close(pipeline[2][1]);
		close(pipeline[3][0]);
		close(pipeline[3][1]);
	}

	if (proc == 1)
	{
		dup2(pipeline[0][0], STDIN_FILENO);
		close(pipeline[0][0]);
		close(pipeline[0][1]);
		close(pipeline[1][0]);
		dup2(pipeline[1][1], STDOUT_FILENO);
		close(pipeline[1][1]);
		close(pipeline[2][0]);
		close(pipeline[2][1]);
		close(pipeline[3][0]);
		close(pipeline[3][1]);
	}
	
	if (proc == 2)
	{
		close(pipeline[0][0]);
		close(pipeline[0][1]);
		dup2(pipeline[1][0], STDIN_FILENO);
		close(pipeline[1][0]);
		close(pipeline[1][1]);
		close(pipeline[2][0]);
		dup2(pipeline[2][1], STDOUT_FILENO);
		close(pipeline[2][1]);
		close(pipeline[3][0]);
		close(pipeline[3][1]);
	}
	
	if (proc == 3)
	{
		close(pipeline[0][0]);
		close(pipeline[0][1]);
		close(pipeline[1][0]);
		close(pipeline[1][1]);
		dup2(pipeline[2][0], STDIN_FILENO);
		close(pipeline[2][0]);
		close(pipeline[2][1]);
		close(pipeline[3][0]);
		dup2(pipeline[3][1], STDOUT_FILENO);
		close(pipeline[3][1]);
	}
	
	if (proc == 4)
	{
		close(pipeline[0][0]);
		close(pipeline[0][1]);
		close(pipeline[1][0]);
		close(pipeline[1][1]);
		close(pipeline[2][0]);
		close(pipeline[2][1]);
		dup2(pipeline[3][0], STDIN_FILENO);
		close(pipeline[3][0]);
		close(pipeline[3][1]);
	}
}*/

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
		return (ft_builtin_pwd(l_env));
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (ft_builtin_cd(cmd->argv, l_env));
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

/*void	ft_exe_pipeline(t_cmd *cmd, t_list *l_env)
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
}*/

// free all pipeline closing only opened pipes
void	ft_free_pipeline(int **pipeline)
{
	int	i;

	if (!pipeline)
		return ;
	i = 0;
	while (pipeline[i] != NULL)
	{
		if (pipeline[i][0] >= 0)
		{
			close(pipeline[i][0]);
			pipeline[i][0] = -1;
		}
		if (pipeline[i][1] >= 0)
		{
			close(pipeline[i][1]);
			pipeline[i][1] = -1;
		}
		free(pipeline[i]);
		i++;
	}
	free(pipeline);
}

// pipeline is a **int null-finished containing int[2] to store pipes
// pipes are opened
int	ft_create_pipes(int ***pipeline, int n_pipes)
{
	int	i;

	*pipeline = (int **) malloc ((n_pipes + 1) * 2 * sizeof(int *));
	if (!*pipeline)
		return (0);
	i = 0;
	while (i < n_pipes)
	{
		(*pipeline)[i] = (int *) malloc (2 * sizeof(int));
		if (!(*pipeline)[i])
			return (0);
		i++;
	}
	(*pipeline)[i] = NULL;
	i = 0;
	while (i < n_pipes)
	{
		if (pipe((*pipeline)[i]) == -1)
			return (0);
		i++;
	}
	return (1);
}

// n_pipes is n_procs - 1
int	ft_calculate_pipes(t_cmd *cmd)
{
	int	n_pipes;
	t_cmd	*current;

	n_pipes = 0;
	current = cmd;
	while (current)
	{
		n_pipes++;
		current = current->next;
	}
	return (n_pipes - 1);
}

int	ft_search_cmd(char *path, t_cmd *cmd)
{
	if (!path)
	{
		if (cmd && cmd->argv && cmd->argv[0])
			ft_printf(STDERR_FILENO, "minishell: %s: command notdfdffdfd found\n", cmd->argv[0]);
		return (0);
	}
	return (1);
}

void	ft_handle_execve_error(char *cmd)
{
	ft_printf(STDERR_FILENO, "minishell: %s: %s\n", cmd, strerror(errno));
	if (errno == ENOENT)
		g_exit_status = 127;
	else if (errno == EACCES)
		g_exit_status = 126;
	else
		g_exit_status = 1;
}

void	ft_proc_files_redir_cmd(t_cmd *cmd)
{
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

void	ft_execute_process(t_cmd *cmd, t_list *l_env)
{
	char	**envp_exec;
	char	*path;

	if (cmd->argv && ft_is_builtin(cmd->argv[0]))
	{
		ft_proc_files_redir_cmd(cmd);
		g_exit_status = ft_execute_builtin(cmd, l_env);
		ft_lstclear(&l_env, ft_free_env);
	}
	else
	{
		envp_exec = ft_build_envp_array(l_env);
		if (cmd->argv)
			path = find_path(cmd->argv[0], envp_exec);
		else
			path = NULL;
		ft_lstclear(&l_env, ft_free_env);
		if (ft_search_cmd(path, cmd))
		{
			ft_proc_files_redir_cmd(cmd);
			if (execve(path, cmd->argv, envp_exec) == -1)
				ft_handle_execve_error(cmd->argv[0]);
		}
		else
			g_exit_status = 127;
		ft_free_tab(envp_exec);
		free(path); // liberar la mem del main (input, token y cmd). provocar el fallo con /usr/bin echo "paco" (con el espacio)
	}
}

int	ft_proc_files_redir_error(t_cmd *cmd, t_list *l_env)
{
	int	ret_val;

	ret_val = 1;
	/*if (cmd->infile > 2)
	{
		dup2(cmd->infile, STDIN_FILENO);
		close(cmd->infile);
	}
	if (cmd->outfile > 2)
	{
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
	}*/
	if (cmd->infile == -1)
	{
		close(STDIN_FILENO);
		ret_val = 0;
	}
	if (cmd->outfile == -1)
	{
		close(STDOUT_FILENO);
		ret_val = 0;
	}
	if (ret_val == 0)
		ft_lstclear(&l_env, ft_free_env);
	return (ret_val);
	// cambio de ultima hora despues de mergear cerrando las STDIN
	// y STDOUT direcmtante. tambien liberando la lista de entorno
	// (ya se libera tres veces por funciones separadas, ver si se
	// puede hacer una sola vez).
	// fake_cmd | wc -l da 1 en vez de 0, probar bien esto por si
	// tiene que ver con estos cambios de ultima hora para evitar leaks
}

void	ft_close_pipes(int **pipeline, int n_pipes)
{
	int	i;

	i = 0;
	while (i < n_pipes)
	{
		if (pipeline[i][0])
			close(pipeline[i][0]);
		if (pipeline[i][1])
			close(pipeline[i][1]);
		i++;
	}
}

void	ft_proc_pipeline_redir(int **pipeline, int proc, int n_procs, t_cmd *cmd)
{
	if (!pipeline || n_procs < 2)
		return ;
	if (proc > 0 && cmd->heredoc_count == 0)
		dup2(pipeline[proc - 1][0], STDIN_FILENO);
	if (proc < (n_procs - 1))
		dup2(pipeline[proc][1], STDOUT_FILENO);
	ft_close_pipes(pipeline, n_procs - 1);
}

// parent needs to close pipes before waiting. OK!
int	ft_create_process(int n_procs, int **pipeline, t_cmd *cmd, t_list *l_env)
{
	int	i;
	pid_t	pid;
	t_cmd	*curr_cmd;
	int	wstatus;

	//pid_t	wpid;
	curr_cmd = cmd;
	i = 0;
	printf("Proc padre: PID = %d\n", getpid());
	while (i < n_procs && curr_cmd)
	{
		ft_manage_heredoc(curr_cmd);
		pid = fork();
		if (pid < 0)
			return (0);
		else if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			ft_proc_pipeline_redir(pipeline, i, n_procs, cmd);
			printf("Proc hijo %d con PID %d\n", i, getpid());fflush(0);
			if (ft_proc_files_redir_error(curr_cmd, l_env))
				ft_execute_process(curr_cmd, l_env);
			ft_free_pipeline(pipeline); // cierro pipes. Incluyo liberacion de mallocs pipeline
			//ft_lstclear(&l_env, ft_free_env); // aunque haya ido mal se habra liberado antes de execve
			ft_cmdclear (&cmd, ft_free_argv);
			exit (g_exit_status);
        	}
        	curr_cmd = curr_cmd->next;
        	i++;
        }
        ft_close_pipes(pipeline, n_procs - 1);
        i = 0;
	while (i < n_procs)
	{
	        //wpid = wait(NULL);
	        if (i == n_procs - 1)
	        {
			wstatus = 0;
			waitpid(pid, &wstatus, 0);
			if (WIFEXITED(wstatus))
				g_exit_status = WEXITSTATUS(wstatus);
			else
			{
				if (WIFSIGNALED(wstatus))
					g_exit_status = 128 + WTERMSIG(wstatus);
			}
		}
		printf("Finalizando...Proc hijo con PID %d ha terminado\n", pid);fflush(0);
		i++;
        }
   	printf("Proc padre: todos los hijos han terminado. PID = %d\n", getpid());
	return (1);
}


// el caso de builtin sin pipes se ha de ejecutar fuera de la pipeline y de los hijos
// para conservar el entorno (los hijos pierden el entorno cuando finalizan


// Ignorar SIGINT en el padre mientras se ejecutan comandos
// calcular n_pipes, crear array de pipes dinamicamente, crear procesos
// liberar array de pipes y restaurar senyales
void	ft_exe_pipeline(t_cmd *cmd, t_list *l_env)
{
	int	n_pipes;
	int	**pipeline;
	int	parent_stdout;
	int	parent_stdin;

	if (!cmd)
		return ;
	
	//valorar si filtrar solo por los builtin que cambian estado y
	// que tienen que ir en el padre
	parent_stdout =  0;
	parent_stdin =  0;
	if (!cmd->next && cmd->argv && ft_is_builtin(cmd->argv[0]))
	{
		if (ft_strcmp(cmd->argv[0], "exit") == 0)
			g_exit_status = ft_execute_builtin(cmd, l_env);
		parent_stdin = dup(STDIN_FILENO);
		parent_stdout = dup(STDOUT_FILENO);
    		ft_manage_heredoc(cmd);
		// no hay pipe, el redir de pipes no tiene sentido, pero ver que
		// hacer si el infile o outfile es -1, es el redir_err de  abajo
		//ft_proc_files_redir_error(curr_cmd, l_env)
		ft_proc_files_redir_cmd(cmd);
    		g_exit_status = ft_execute_builtin(cmd, l_env);
    		dup2(parent_stdin, STDIN_FILENO);
    		dup2(parent_stdout, STDOUT_FILENO); // Restauramos STDOUT
    		close(parent_stdin);
    		close(parent_stdout);
		return ;
	}
	
	
	ft_setup_signals(0);
	n_pipes = ft_calculate_pipes(cmd);
	ft_printf(STDOUT_FILENO, "\nnumero de pipes %d\n", n_pipes);
	
	//(void) pipeline; //QUITAR ESTO PARA PROBAR TEMA LEAKS
	// ME HE QUEDADO EN ESTE BLOQUE PARA EL TEMA LEAKS
	// PERO FALTA PROBAR EL EXIT DE LOS BUILTIN Y TODOS CON EL SANITIZE
	
	if (!ft_create_pipes(&pipeline, n_pipes))
	{
		perror("minishell: creating pipe error");
		g_exit_status = 1;
		ft_free_pipeline(pipeline);
		ft_cmdclear (&cmd, ft_free_argv);
		ft_lstclear(&l_env, ft_free_env);
		exit (g_exit_status);
	}
	for(int i=0; i<n_pipes; i++)
		ft_printf(1,"Pipe %d: [0]%d [1]%d\n",i, pipeline[i][0], pipeline[i][1]);
	if (!ft_create_process(n_pipes + 1, pipeline, cmd, l_env))
	{
		perror("minishell: creating fork error");
		g_exit_status = 1;
		ft_free_pipeline(pipeline);
		ft_cmdclear (&cmd, ft_free_argv);
		ft_lstclear(&l_env, ft_free_env);
		exit (g_exit_status);
	}
	ft_free_pipeline(pipeline);
	ft_setup_signals(1);
}


