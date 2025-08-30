/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:32:54 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/30 17:15:37 by mgrandia         ###   ########.fr       */
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

static void	heredoc_init(struct sigaction *sa_old, t_n *here_norm)
{
	struct sigaction	sa_heredoc;

	here_norm->line = NULL;
	here_norm->flag = 0;
	here_norm->i_heredoc = 0;
	sigaction(SIGINT, NULL, sa_old);
	sa_heredoc.sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa_heredoc.sa_mask);
	sa_heredoc.sa_flags = 0;
	sigaction(SIGINT, &sa_heredoc, NULL);
	disable_sigquit();
}

static void	ft_heredoc_close(struct sigaction *sa_old)
{
	sigaction(SIGINT, sa_old, NULL);
	enable_sigquit();
}

int	ft_cr_hdoc(t_cmd *cmd, t_list *l_env)
{
	int					pipefd[2];
	struct sigaction	*sa_old;
	t_n					here_norm;

	sa_old = NULL;
	heredoc_init(sa_old, &here_norm);
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
			return (h_err_line(sa_old, pipefd, cmd));
		h_manage_1(here_norm, cmd, pipefd, l_env);
		if (h_mng2(&here_norm, cmd, sa_old, pipefd))
			return (pipefd[0]);
		free(here_norm.line);
	}
	ft_heredoc_close(sa_old);
	close(pipefd[1]);
	return (pipefd[0]);
}
