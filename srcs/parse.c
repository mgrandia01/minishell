/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/13 14:46:40 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//Si ves TOKEN_REDIR_IN (<), el siguiente token debería ser un WORD con el nombre del archivo, y lo guardas en cmd->infile (abriendo el archivo y guardando el fd).

//Si ves TOKEN_REDIR_OUT (>), igual, abres archivo en modo truncado y guardas en cmd->outfile.

//Si ves TOKEN_REDIR_APPEND (>>), abres archivo en modo append y guardas en cmd->outfile.

//Si ves TOKEN_HEREDOC (<<), guardas el delimitador en cmd->heredoc_delim y marcas cmd->heredoc = 1.

//TODO mensaje de error amb els return ;??
int	argv_num(char **argv)
{
	int	num;

	num = 0;
	while (argv && argv[num])
		num ++;
	return (num);
}

/*
void	add_word(t_cmd *cmd, char *word)
{
	int		s;
	int		s2;
	char	**n_a;
	char	*dup_word;

	dup_word = ft_strdup(word);
	if (!dup_word)
		return ;
	s = argv_num(cmd->argv);
	s2 = s + 2;
	if (cmd -> argv)
	{
		n_a = ft_realloc(cmd->argv, s * sizeof(char *), s2 * sizeof(char *));
		if (!n_a)
		{
			free(dup_word);
			return ;
		}
		cmd -> argv = n_a;
	}
	else
	{
		cmd -> argv = malloc(2 * sizeof(char *));
		if (!cmd -> argv)
		{
			free(dup_word);
			return ;
		}
	}
	cmd -> argv[s] = dup_word;
	cmd -> argv[s + 1] = NULL;
	ft_printf("a size = %d, tenim = %s\n", s, cmd -> argv[s]);
}*/

static char	**expand_argv(char **argv, int s)
{
	char	**n_argv;

	if (argv)
		n_argv = ft_realloc(argv, s * sizeof(char *), (s + 2) * sizeof(char *));
	else
		n_argv = malloc(2 * sizeof(char *));
	return (n_argv);
}

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

t_cmd	*init_comand(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (0);
	cmd -> argv = NULL;
	cmd -> infile = 0;
	cmd -> outfile = 0;
	cmd -> heredoc = 0;
	cmd -> heredoc_delim = NULL;
	cmd -> next = NULL;
	return (cmd);
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

void	handle_pipe_token(t_cmd *cmd, t_redir_type *expect_redir)
{
	cmd -> next = init_comand();
	cmd = cmd -> next;
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
			handle_pipe_token(cmd, &expect_redir);
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
