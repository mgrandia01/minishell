/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 11:09:50 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/30 17:05:31 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_line_expanded(t_token *tokens, char *l_exp2)
{
	char	*l_exp1;

	l_exp1 = NULL;
	while (tokens && ft_strncmp(tokens->value, "EOF", 3))
	{
		if (tokens->value)
		{
			if (l_exp2 != NULL)
			{
				free(l_exp2);
				l_exp2 = ft_join_triple(l_exp1, " ", tokens->value);
				free(l_exp1);
			}
			else
				l_exp2 = ft_strdup(tokens->value);
			l_exp1 = ft_strdup(l_exp2);
		}
		tokens = tokens->next;
	}
	if (l_exp1)
		free(l_exp1);
	if (!l_exp2)
		l_exp2 = ft_calloc(1, sizeof(char));
	return (l_exp2);
}

static char	*ft_expanse_heredoc(char *line, t_list *l_env)
{
	t_token	*tokens;
	char	**envp_exec;
	char	*line_expansed2;

	tokens = ft_tokenize(line, 1);
	line_expansed2 = NULL;
	if (!tokens)
		return (line);
	envp_exec = ft_build_envp_array(l_env);
	if (!envp_exec)
	{
		free_tokens(tokens);
		tokens = NULL;
		return (line);
	}
	process_token_exp(&tokens, envp_exec, 1);
	join_tokens_with_end(&tokens);
	line_expansed2 = ft_line_expanded(tokens, line_expansed2);
	ft_free_tab(envp_exec);
	free_tokens(tokens);
	return (line_expansed2);
}

void	heredoc_init(struct sigaction *sa_old, t_n *here_norm)
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

void	ft_heredoc_close(struct sigaction *sa_old)
{
	sigaction(SIGINT, sa_old, NULL);
	enable_sigquit();
}

int	h_err_line(struct sigaction *sa_old, int pipefd[2], t_cmd *cmd)
{
	int	i;

	sigaction(SIGINT, sa_old, NULL);
	close(pipefd[1]);
	close(pipefd[0]);
	if (g_exit_status == 130)
	{
		i = 0;
		while (i < cmd->outfile_count)
		{
			unlink(cmd->outfile_name[i]);
			i++;
		}
	}
	return (-1);
}

void	h_manage_1(t_n h_nor, t_cmd *cmd, int *pipefd, t_list *l_env)
{
	char	*line_expansed;

	line_expansed = NULL;
	if (ft_strchr(h_nor.line, '\n'))
		*ft_strchr(h_nor.line, '\n') = '\0';
	if (((h_nor.flag == 1) || cmd->heredoc_count == 1)
		&&!(ft_strcmp(h_nor.line, cmd->heredocs[h_nor.i_heredoc].delimiter) == 0
			&& (h_nor.i_heredoc == cmd->heredoc_count - 1)))
	{
		line_expansed = ft_expanse_heredoc(h_nor.line, l_env);
		write(pipefd[1], line_expansed, ft_strlen(line_expansed));
		write(pipefd[1], "\n", 1);
	}
	if (line_expansed != NULL)
		free(line_expansed);
}

int	h_mng2(t_n *h_nor, t_cmd *cmd, struct sigaction *sa_old, int pipefd[2])
{
	if (ft_strcmp(h_nor->line, cmd->heredocs[h_nor->i_heredoc].delimiter) == 0)
	{
		if (h_nor->i_heredoc < cmd->heredoc_count - 2)
			(h_nor->i_heredoc)++;
		else if (h_nor->i_heredoc == cmd->heredoc_count - 2)
		{
			h_nor->flag = 1;
			(h_nor->i_heredoc)++;
		}
		else if (h_nor->i_heredoc == cmd->heredoc_count - 1)
		{
			free(h_nor->line);
			sigaction(SIGINT, sa_old, NULL);
			close(pipefd[1]);
			return (1);
		}
	}
	return (0);
}
