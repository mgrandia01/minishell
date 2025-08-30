/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 08:56:32 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/30 11:32:03 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* Checks for errors when opening redirection files.
 * Prints an error message if a file descriptor is invalid.
 */
void	handle_errfile(t_cmd *cmd, t_redir_type *expect_redir, char *v)
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

void	open_redir_file(int *fd, const char *filename, int mode)
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

void	create_outfile_name(t_cmd *cmd, const char *filename)
{
	cmd->outfile_name = malloc(sizeof(char *));
	if (cmd->outfile_name != NULL)
	{
		cmd->outfile_name[0] = ft_strdup(filename);
		cmd->outfile_count = 1;
	}
}

void	add_outfile_name(t_cmd *cmd, const char *filename)
{
	char	**new_array;
	int		i;

	i = 0;
	if (!cmd || !filename)
		return ;
	if (cmd->outfile_name == NULL)
		create_outfile_name(cmd, filename);
	else
	{
		new_array = malloc(sizeof(char *) * (cmd->outfile_count + 2));
		if (new_array != NULL)
		{
			while (i < cmd->outfile_count)
			{
				new_array[i] = cmd->outfile_name[i];
				i++;
			}
			new_array[cmd->outfile_count] = ft_strdup(filename);
			free(cmd->outfile_name);
			cmd->outfile_name = new_array;
			cmd->outfile_count++;
			new_array[cmd->outfile_count] = NULL;
		}
	}
}

int	find_mode(t_redir_type *expect_redir)
{
	if (*expect_redir == OUTFILE)
		return (2);
	else
		return (3);
}
