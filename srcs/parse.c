/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/22 08:58:37 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Duplicates and adds a word to the command's argv array.
 * Expands the array if needed and null-terminates it.
 */
static void	add_word(t_cmd *cmd, char *word)
{
	char	*dup_word;
	int		size;

	if (!word || *word == '\0')
		return ;
	dup_word = ft_strdup(word);
	if (!dup_word)
		return ;
	size = argv_num(cmd->argv);
	cmd->argv = expand_argv(cmd->argv, size);
	if (!cmd->argv)
	{
		free(dup_word);
		return ;
	}
	cmd->argv[size] = dup_word;
	cmd->argv[size + 1] = NULL;
}

void	add_heredoc(t_cmd *cmd, const char *delimiter)
{
	t_heredoc	*new_array;
	int			i;

	new_array = malloc(sizeof(t_heredoc) * (cmd->heredoc_count + 1));
	i = 0;
	if (!new_array)
		return ;
	while (i < cmd->heredoc_count)
	{
		new_array[i] = cmd->heredocs[i];
		i++;
	}
	new_array[cmd->heredoc_count].delimiter = ft_strdup(delimiter);
	free(cmd->heredocs);
	cmd->heredocs = new_array;
	cmd->heredoc_count++;
}

/* Handles a word token depending on the expected redirection type.
 * Opens files for redirections or adds the word to the command.
 * Resets expected redirection type after processing.
 */
static void	handle_word(t_cmd *cmd, t_token *tokens, t_redir_type *expect_redir)
{
	int	mode;

	mode = 0;
	if (*expect_redir == INFILE)
	{
		if (cmd->error)
			return ;
		open_redir_file(&cmd->infile, tokens->value, 1);
		handle_error_file(cmd, expect_redir, tokens->value);
	}
	else if (*expect_redir == OUTFILE || *expect_redir == APPEND)
	{
		mode = find_mode(expect_redir);
		if (cmd->error)
			return ;
		open_redir_file(&cmd->outfile, tokens->value, mode);
		handle_error_file(cmd, expect_redir, tokens->value);
		add_outfile_name(cmd, tokens->value);
	}
	else if (*expect_redir == HEREDOC)
		add_heredoc(cmd, tokens->value);
	else
		add_word(cmd, tokens->value);
	*expect_redir = NONE;
}

/* Handles a pipe token by creating a new command node.
 * Resets expected redirection type.
 */
static void	handle_pipe_token(t_cmd **cmd, t_redir_type *expect_redir)
{
	(*cmd)-> next = init_comand();
	*cmd = (*cmd)-> next;
	*expect_redir = NONE;
}

/* Parses the list of tokens into a command structure.
 * Handles pipes, redirections, and command words.
 * Returns the head of the command list.
 */
struct s_cmd	*ft_parse(t_token *tokens)
{
	t_cmd			*cmd_head;
	t_cmd			*cmd;
	t_redir_type	expect_redir;

	cmd_head = init_comand();
	cmd = cmd_head;
	expect_redir = NONE;
	while (tokens)
	{
		if ((tokens->type) == TOKEN_PIPE)
			handle_pipe_token(&cmd, &expect_redir);
		else if ((tokens->type) == TOKEN_REDIR_IN)
			expect_redir = INFILE;
		else if ((tokens->type) == TOKEN_REDIR_OUT)
			expect_redir = OUTFILE;
		else if ((tokens->type) == TOKEN_REDIR_APPEND)
			expect_redir = APPEND;
		else if ((tokens->type) == TOKEN_HEREDOC)
			expect_redir = HEREDOC;
		else if ((tokens->type) == TOKEN_WORD)
			handle_word(cmd, tokens, &expect_redir);
		tokens = tokens -> next;
	}
	return (cmd_head);
}
