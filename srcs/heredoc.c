/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:32:54 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/22 10:54:17 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_count_heredocs(t_token *tokens)
{
	int	count;

	count = 0;
	while(tokens)
	{
		if (tokens->type == TOKEN_HEREDOC)
			count ++;
		tokens = tokens->next;
	}
	return (count);
}

static char	*ft_expanse_heredoc(char *line, t_list *l_env)
{
	t_token	*tokens;
	char	**envp_exec;
	char	*line_expansed1;
	char	*line_expansed2;

	tokens = ft_tokenize(line);
	if (!tokens)
		return (line);
	envp_exec = ft_build_envp_array(l_env);
	if (!envp_exec) // check tras crear envp
	{
		free_tokens(tokens);
		tokens = NULL;
		return (line);
    	}
	
	
	// hasta no expandimos nada porque habra fallado el tokenizador
	// aqui ya expandimos
	
	process_token_expansion(&tokens, envp_exec);
	join_tokens_with_end(&tokens);
	remove_quotes_from_token_list(tokens);
		
	line_expansed2 = NULL;
	line_expansed1 = NULL;
	while(tokens && ft_strncmp(tokens->value, "EOF", 3))
	{
		if (tokens->value)
		{
			if (line_expansed2 != NULL)
			{
				line_expansed2 = ft_strjoin(line_expansed1, " ");
				free(line_expansed1);
				line_expansed1 = ft_strdup(line_expansed2);
				line_expansed2 =  ft_strjoin(line_expansed1, tokens->value);
				free(line_expansed1);
			}
			else
			{
				line_expansed2 =  ft_strdup(tokens->value);
			}
			
			line_expansed1 = ft_strdup(line_expansed2);
		}
		tokens = tokens->next;
	}
	if (line_expansed1)
		free(line_expansed1);
	if (!line_expansed2)
		line_expansed2 = (char *)malloc(sizeof(char));
	ft_free_tab(envp_exec);
	free_tokens(tokens);
	return (line_expansed2);
	
}

int	ft_create_heredoc(t_heredoc *delim, int heredoc_count, t_cmd *cmd, t_list *l_env)
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
	(void)cmd;//TODO finalmente, se puede borrar?
	sigaction(SIGINT, NULL, &sa_old);
	sa_heredoc.sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa_heredoc.sa_mask);
	sa_heredoc.sa_flags = 0;
	sigaction(SIGINT, &sa_heredoc, NULL);
	disable_sigquit();
	sigaction(SIGQUIT, NULL, &sa_old_quit);
	sa_heredoc_quit.sa_handler = sigquit_handler_heredoc;
	sigemptyset(&sa_heredoc_quit.sa_mask);
	sa_heredoc_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_heredoc_quit, NULL);
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
				//free(line_expansed);
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
	close(pipefd[1]);
	return (pipefd[0]);
}
