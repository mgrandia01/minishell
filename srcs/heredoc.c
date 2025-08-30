/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:32:54 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/28 16:02:38 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_count_heredocs(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_HEREDOC)
			count ++;
		tokens = tokens->next;
	}
	return (count);
}

int	ft_cr_hdoc(t_cmd *cmd, t_list *l_env)
{
	int					pipefd[2];
	struct sigaction	*sa_old;
	t_here_norm			here_norm;

	sa_old = NULL;
	ft_heredoc_init(sa_old, &here_norm);
	if (pipe(pipefd) == -1)
	{
		sigaction(SIGINT, sa_old, NULL);
		return (-1);
	}
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		here_norm.line = get_next_line(STDIN_FILENO);
		if (!here_norm.line)
			return (ft_heredoc_error_line(sa_old, pipefd, cmd));
		ft_heredoc_manage_line_1(here_norm, cmd, pipefd, l_env);
		if (ft_heredoc_manage_line_2(&here_norm, cmd, sa_old, pipefd))
			return (pipefd[0]);
		free(here_norm.line);
	}
	ft_heredoc_close(sa_old);
	close(pipefd[1]);
	return (pipefd[0]);
}

/*int	ft_cr_hdoc(t_heredoc *delim, int heredoc_count, t_cmd *cmd, t_list *l_env)
{
	int							pipefd[2];
	char						*line;
	char						*line_expansed;
	struct sigaction			sa_heredoc;
	struct sigaction			sa_heredoc_quit;
	struct sigaction			sa_old;
	struct sigaction			sa_old_quit;
	int							i_heredoc;
	int							flag;
	int							i;

	flag = 0;
	i_heredoc = 0;
	i = 0;
	
	sigaction(SIGINT, NULL, &sa_old);
	sa_heredoc.sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa_heredoc.sa_mask);
	sa_heredoc.sa_flags = 0;
	sigaction(SIGINT, &sa_heredoc, NULL);
	
	sigaction(SIGQUIT, NULL, &sa_old_quit);
	sa_heredoc_quit.sa_handler = sigquit_handler_heredoc;
	sigemptyset(&sa_heredoc_quit.sa_mask);
	sa_heredoc_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_heredoc_quit, NULL);
	disable_sigquit();
	
	if (pipe(pipefd) == -1)
	{
		sigaction(SIGINT, &sa_old, NULL);
		sigaction(SIGQUIT, &sa_old_quit, NULL);
		return (-1);
	}
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			sigaction(SIGINT, &sa_old, NULL);
			sigaction(SIGQUIT, &sa_old_quit, NULL);
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
			line_expansed = ft_expanse_heredoc(line, l_env);
			write(pipefd[1], line_expansed, ft_strlen(line_expansed));
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
				sigaction(SIGQUIT, &sa_old_quit, NULL);
				close(pipefd[1]);
				return (pipefd[0]);
			}
		}
		free(line);
		free(line_expansed);
	}
	sigaction(SIGINT, &sa_old, NULL);
	sigaction(SIGQUIT, &sa_old_quit, NULL);
	enable_sigquit();
	close(pipefd[1]);
	return (pipefd[0]);
}*/

