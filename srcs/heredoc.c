/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:32:54 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/18 16:24:11 by mgrandia         ###   ########.fr       */
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

int	ft_create_heredoc(t_heredoc *delim, int heredoc_count, t_cmd *cmd)
{
	int							pipefd[2];
	char						*line;
	struct sigaction			sa_heredoc;
	struct sigaction			sa_old;
	int							i_heredoc;
	int							flag;
	int							i;

	flag = 0;
	i_heredoc = 0;
	i = 0;
	(void)cmd;//TODO finalmente, se puede borrar?
	sigaction(SIGINT, NULL, &sa_old);
	sa_heredoc.sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa_heredoc.sa_mask);
	sa_heredoc.sa_flags = 0;
	sigaction(SIGINT, &sa_heredoc, NULL);
	if (pipe(pipefd) == -1)
	{
		sigaction(SIGINT, &sa_old, NULL);
		return (-1);
	}
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			sigaction(SIGINT, &sa_old, NULL);
			close(pipefd[1]);
			close(pipefd[0]);
			if (g_exit_status == 130)
			{
				while (i < cmd->outfile_count)
				{
					unlink(cmd->outfile_name[i]);
					i++;
				}
			}
			return (-1);
		}
		if (ft_strchr(line, '\n'))
			*ft_strchr(line, '\n') = '\0';
		if ((flag == 1 || heredoc_count == 1)
			&&!(ft_strcmp(line, delim[i_heredoc].delimiter) == 0
				&& (i_heredoc == heredoc_count - 1)))
		{
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
		}
		if (ft_strcmp(line, delim[i_heredoc].delimiter) == 0)
		{
			if (i_heredoc < heredoc_count - 2)
				i_heredoc++;
			else if (i_heredoc == heredoc_count - 2)
			{
				flag = 1;
				i_heredoc++;
			}
			else if (i_heredoc == heredoc_count - 1)
			{
				free(line);
				sigaction(SIGINT, &sa_old, NULL);
				close(pipefd[1]);
				return (pipefd[0]);
			}
		}
		free(line);
	}
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
