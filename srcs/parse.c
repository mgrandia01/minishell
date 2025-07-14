/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/14 11:34:58 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//Si ves TOKEN_REDIR_IN (<), el siguiente token debería ser un WORD con el nombre del archivo, y lo guardas en cmd->infile (abriendo el archivo y guardando el fd).

//Si ves TOKEN_REDIR_OUT (>), igual, abres archivo en modo truncado y guardas en cmd->outfile.

//Si ves TOKEN_REDIR_APPEND (>>), abres archivo en modo append y guardas en cmd->outfile.

//Si ves TOKEN_HEREDOC (<<), guardas el delimitador en cmd->heredoc_delim y marcas cmd->heredoc = 1.

//TODO mensaje de error amb els return ;??
void	add_word(t_cmd *cmd, char *word)
{
	char	*dup_word;
	int		size;

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

void	handle_error_file(t_cmd *cmd, t_redir_type *expect_redir)
{
	if (*expect_redir == INFILE)
	{
		if (cmd -> infile < 0)
			perror("open infile");
	}
	else if (*expect_redir == OUTFILE)
	{
		if (cmd -> outfile < 0)
			perror("open outfile");
	}
	else if (*expect_redir == APPEND)
	{
		if (cmd -> outfile < 0)
			perror("open outfile append");
	}
}

//TODO mirar lo de perror donde sale en bash
void	handle_word(t_cmd *cmd, t_token *tokens, t_redir_type *expect_redir)
{
	if (*expect_redir == INFILE)
	{
		cmd->infile = open(tokens->value, O_RDONLY);
		handle_error_file(cmd, expect_redir);
	}
	else if (*expect_redir == OUTFILE)
	{
		cmd->outfile = open(tokens->value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		handle_error_file(cmd, expect_redir);
	}
	else if (*expect_redir == APPEND)
	{
		cmd->outfile = open(tokens->value, O_CREAT | O_WRONLY | O_APPEND, 0644);
		handle_error_file(cmd, expect_redir);
	}
	else if (*expect_redir == HEREDOC)
	{
		cmd -> heredoc = 1;
		cmd -> heredoc_delim = strdup(tokens->value);
	}
	else
		add_word(cmd, tokens->value);
	*expect_redir = NONE;
}

void	handle_pipe_token(t_cmd **cmd, t_redir_type *expect_redir)
{
	(*cmd) -> next = init_comand();
	*cmd = (*cmd) -> next;
	*expect_redir = NONE;
}

//TODO comillas dentro de el argv?
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
