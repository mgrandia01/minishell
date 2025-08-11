/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:32:54 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/02 10:32:59 by arcmarti         ###   ########.fr       */
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

static void sigint_handler_heredoc(int sig)
{
	(void) sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130); // Bash devuelve 130 en este caso
}

int	ft_create_heredoc(t_heredoc *delim, int heredoc_count)
{
	int	pipefd[2];
	char	*line;
	struct sigaction sa;
	int	i_heredoc;
	int	flag = 0;
	
	
	sa.sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	i_heredoc = 0;
	printf("\nANTES BUCLE %d \n", i_heredoc);
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
		/*if (ft_strcmp(line, delim[0].delimiter) == 0)
		{
			free(line);
			break ;
		}*/
		if (
		(flag == 1 || heredoc_count == 1) && 
		
		!(ft_strcmp(line, delim[i_heredoc].delimiter) == 0 && (i_heredoc == heredoc_count-1))
		
		)
		{
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
		}
	
		printf("\n estoy en HD> i_hrere %d", i_heredoc);fflush(0);
		if (ft_strcmp(line, delim[i_heredoc].delimiter) == 0)
		{
			if (i_heredoc < heredoc_count - 2 )
			{
				i_heredoc++;
			}
			else if (i_heredoc == heredoc_count - 2)
			{
				flag = 1;
				//write(pipefd[1], line, ft_strlen(line));
				//write(pipefd[1], "\n", 1);
				i_heredoc++;

			//ir guardando
			}
			else if (i_heredoc == heredoc_count-1)
			{
				free(line);
				break ;//salir
			}	
		}
		free(line);
	}
	close(pipefd[1]); // cerramos escritura, mantenemos lectura
	return (pipefd[0]); // devolvemos el fd de lectura
}

