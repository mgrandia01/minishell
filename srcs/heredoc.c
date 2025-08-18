/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:32:54 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/12 13:14:05 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_heredoc(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd)
	{
		while (i < cmd->heredoc_count)
		{
			free((cmd->heredocs[i]).delimiter);
			i++;
		}
		free(cmd->heredocs);
	}
}


static void	sigint_handler_heredoc(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	/*if (isatty(STDIN_FILENO))
        	ft_printf(STDOUT_FILENO,"\033[1;32mminishell \u25B8\033[0m ");
        g_exit_status = 130;*/
        g_exit_status = 130;
}

int	ft_create_heredoc(t_heredoc *delim, int heredoc_count)
{
	int					pipefd[2];
	char				*line;
	struct sigaction	sa_heredoc;
	struct sigaction	sa_old;
	int					i_heredoc;
	int					flag;
	
	flag = 0;
	i_heredoc = 0;
	
	// Guardar el handler actual (tu ft_handle_sigint)
	sigaction(SIGINT, NULL, &sa_old);
	
	// Configurar handler para heredoc SIN SA_RESTART
	sa_heredoc.sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa_heredoc.sa_mask);
	sa_heredoc.sa_flags = 0;  // CRÍTICO: Sin SA_RESTART para interrumpir read()
	sigaction(SIGINT, &sa_heredoc, NULL);
		
	if (pipe(pipefd) == -1)
	{
		sigaction(SIGINT, &sa_old, NULL);
		return (-1);
	}
	
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		
		// get_next_line se interrumpirá con NULL si llega SIGINT
		line = get_next_line(STDIN_FILENO);
		
		if (!line)
		{
			// get_next_line retornó NULL
			// Puede ser EOF (Ctrl+D) o interrupción (Ctrl+C)
			
			// Restaurar handler original INMEDIATAMENTE
			sigaction(SIGINT, &sa_old, NULL);
			
			close(pipefd[1]);
			close(pipefd[0]);
			
			// Si fue interrupción por SIGINT, errno debería ser EINTR
			// Pero como get_next_line puede resetear errno, asumimos que fue Ctrl+C
			// si no hay más datos disponibles
			
			//g_exit_status = 130;
			return (-1);
		}
		
		// Si llegamos aquí, tenemos una línea válida
		
		// Sacar el salto de línea si lo hay
		if (ft_strchr(line, '\n'))
			*ft_strchr(line, '\n') = '\0';
		
		// Escribir al pipe solo si estamos en el heredoc correcto
		if ((flag == 1 || heredoc_count == 1)
			&&!(ft_strcmp(line, delim[i_heredoc].delimiter) == 0
				&& (i_heredoc == heredoc_count - 1)))	
		{
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
		}
		
		// Verificar si es un delimiter
		if (ft_strcmp(line, delim[i_heredoc].delimiter) == 0)
		{
			if (i_heredoc < heredoc_count - 2)
			{
				i_heredoc++;
			}
			else if (i_heredoc == heredoc_count - 2)
			{
				flag = 1;
				i_heredoc++;
			}
			else if (i_heredoc == heredoc_count - 1)
			{
				// Último delimiter encontrado - éxito
				free(line);
				sigaction(SIGINT, &sa_old, NULL);  // Restaurar handler
				close(pipefd[1]);
				return (pipefd[0]);  // Retornar fd válido
			}
		}
		
		free(line);
	}
	
	// No debería llegar aquí nunca
	sigaction(SIGINT, &sa_old, NULL);
	close(pipefd[1]);
	return (pipefd[0]);
}
/*static void	sigint_handler_heredoc(int sig)
{
	(void) sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130); // Bash devuelve 130 en este caso
}*/









/*
// Función auxiliar 1: Configurar señales
static void	ft_setup_sig(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
}

// Función auxiliar 2: Procesar línea de entrada
static char	*ft_process_input_line(void)
{
	char	*line;
	char	*newline_pos;

	write(STDOUT_FILENO, "> ", 2);
	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	newline_pos = ft_strchr(line, '\n');
	if (newline_pos)
		*newline_pos = '\0';
	return (line);
}

// Función auxiliar 3: Escribir línea al pipe si corresponde
static void	ft_w_pipe(int pipefd_w, char *line, t_heredoc *del, int i_heredoc)
{
	int	line_len;
	int	is_final_delimiter;

	is_final_delimiter = ft_strcmp(line, del[i_heredoc].delimiter) == 0;
	if (!is_final_delimiter)
	{
		line_len = ft_strlen(line);
		write(pipefd_w, line, line_len);
		write(pipefd_w, "\n", 1);
	}
}

// Función auxiliar 4: Verificar si es delimitador
static int	ft_is_delimiter(char *line, t_heredoc *delim, int i_heredoc)
{
	int	result;

	result = ft_strcmp(line, delim[i_heredoc].delimiter);
	return (result == 0);
}

// Función auxiliar 5: Actualizar índice de heredoc
static int	ft_update_h_ind(int *i_heredoc, int heredoc_count, int *flag)
{
	if (*i_heredoc < heredoc_count - 2)
	{
		(*i_heredoc)++;
		return (0);
	}
	else if (*i_heredoc == heredoc_count - 2)
	{
		*flag = 1;
		(*i_heredoc)++;
		return (0);
	}
	else
		return (1);
}

// Función auxiliar 6: Procesar bucle principal
static int	ft_pro_h_loop(int pipefd, t_heredoc *delim, int h_count, int flag)
{
	char	*line;
	int		i_heredoc;
	int		should_break;

	i_heredoc = 0;
	while (1)
	{
		line = ft_process_input_line();
		if (!line)
			break ;
		if ((flag == 1 || h_count == 1))
			ft_w_pipe(pipefd, line, delim, i_heredoc);
		if (ft_is_delimiter(line, delim, i_heredoc))
		{
			should_break = ft_update_h_ind(&i_heredoc, h_count, &flag);
			if (should_break)
			{
				free(line);
				break ;
			}
		}
		free(line);
	}
	return (0);
}

// Función principal refactorizada
int	ft_create_heredoc(t_heredoc *delim, int heredoc_count)
{
	int	pipefd[2];
	int	pipe_result;

	ft_setup_sig();
	pipe_result = pipe(pipefd);
	if (pipe_result == -1)
		return (-1);
	ft_pro_h_loop(pipefd[1], delim, heredoc_count, 0);
	close(pipefd[1]);
	return (pipefd[0]);
}
*/
/*int	ft_create_heredoc(t_heredoc *delim, int heredoc_count)
{
	int					pipefd[2];
	char				*line;
	struct sigaction	sa;
	int					i_heredoc;
	int					flag;
	struct sigaction	old_sa;  // Guardar el handler anterior

	flag = 0;
	i_heredoc = 0;
	
	sa.sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, &old_sa);  // Guardar el handler anterior
	//sigaction(SIGINT, &sa, NULL);
	

	
	if (pipe(pipefd) == -1)
		return (-1);
	//printf("\nee HEREDOC %d eee \n", i_heredoc);
	while (1)
	{
		//tot aixo nomes necesita line
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		// Sacar el salto de línea si lo hay
		if (ft_strchr(line, '\n'))
			*ft_strchr(line, '\n') = '\0';
		//
		if ((flag == 1 || heredoc_count == 1)
			&&!(ft_strcmp(line, delim[i_heredoc].delimiter) == 0
				&& (i_heredoc == heredoc_count - 1)))	
		{
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
		}
		
		if (ft_strcmp(line, delim[i_heredoc].delimiter) == 0)
		{
			//printf("\neeeeeeeeeDelim  %s - i_heredoc %d - count %d \n", delim[0].delimiter, i_heredoc, heredoc_count);
			//tot aixo nomes necessita o_heredoc, heredoc_count, flag i line
			if (i_heredoc < heredoc_count - 2)
			{
				i_heredoc++;
			}
			else if (i_heredoc == heredoc_count - 2)
			{
				flag = 1;
				i_heredoc++;
			}
			else if (i_heredoc == heredoc_count - 1)
			{
				free(line);
				break ;
			}
			//
		}
		free(line);
	}
	
	sigaction(SIGINT, &old_sa, NULL);
	//printf("\neeeeeeeeeeeeeeeeeeeeee %d\n", pipefd[0]);fflush(0);
	close(pipefd[1]); // cerramos escritura, mantenemos lectura
	return (pipefd[0]); // devolvemos el fd de lectura
}*/

