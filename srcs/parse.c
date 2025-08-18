/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/18 17:06:35 by mgrandia         ###   ########.fr       */
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

/* Checks for errors when opening redirection files.
 * Prints an error message if a file descriptor is invalid.
 */
static void	handle_error_file(t_cmd *cmd, t_redir_type *expect_redir, char *v)
{
	int	fd;

	fd = -1;
	if (*expect_redir == INFILE)
		fd = cmd -> infile; 
	if (*expect_redir == OUTFILE || *expect_redir == APPEND)
		fd = cmd -> outfile; 
	if (fd < 0)
	{
		ft_printf(STDERR_FILENO, "minishell: %s: %s\n", v, strerror(errno));
		cmd->error = 1;
		cmd -> outfile = -1;
	}
}

static void	open_redir_file(int *fd, const char *filename, int mode)
{
	if (*fd > 2)
		close(*fd);
	if (mode == 1)
		*fd = open(filename, O_RDONLY);
	else if (mode == 2)
		*fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (mode == 3)
		*fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
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

//TODO liberacion de outfile_name de la funcion add_outfile_name
void	free_outfiles(t_cmd *cmd)
{
	int i;

	i = 0;
	if (!cmd || !cmd->outfile_name)
		return ;

	while (i < cmd->outfile_count)
	{
		free(cmd->outfile_name[i]);
		i++;
	}
	free(cmd->outfile_name);
	cmd->outfile_name = NULL;
	cmd->outfile_count = 0;
}

static void	add_outfile_name(t_cmd *cmd, const char *filename)
{
	char	**new_array;
	int		i;

	i = 0;
	if (!cmd || !filename)
		return ;
	if (cmd->outfile_name == NULL)
	{
		//TODO free outfiles
		cmd->outfile_name = malloc(sizeof(char *));
		if (cmd->outfile_name != NULL)
		{
			cmd->outfile_name[0] = ft_strdup(filename);
			cmd->outfile_count = 1;
		}
	}
	else
	{
		new_array = malloc(sizeof(char *) * (cmd->outfile_count + 1));
		if (new_array != NULL)
		{
			while ( i < cmd->outfile_count)
			{
				new_array[i] = cmd->outfile_name[i];
				i++;
			}
			new_array[cmd->outfile_count] = ft_strdup(filename);
			free(cmd->outfile_name);
			cmd->outfile_name = new_array;
			cmd->outfile_count++;
		}
	}
}


/* Handles a word token depending on the expected redirection type.
 * Opens files for redirections or adds the word to the command.
 * Resets expected redirection type after processing.
//TODO si hi ha més d'un < en un costat del pipex, obrirlos tots pero
//només passar l'ultim! fer una prova printant el infile a veure que passa
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
		if (*expect_redir == OUTFILE)
			mode = 2;
		else
			mode = 3;
		if (cmd->error)
			return ;
	
		open_redir_file(&cmd->outfile, tokens->value, mode);
		handle_error_file(cmd, expect_redir, tokens->value);
		add_outfile_name(cmd, tokens->value);
	}

	else if (*expect_redir == HEREDOC)
	{
		add_heredoc(cmd, tokens->value);
	}
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
