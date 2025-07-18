/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 16:48:20 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/14 11:25:24 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	argv_num(char **argv)
{
	int	num;

	num = 0;
	while (argv && argv[num])
		num ++;
	return (num);
}

char	**expand_argv(char **argv, int s)
{
	char	**n_argv;

	if (argv)
		n_argv = ft_realloc(argv, s * sizeof(char *), (s + 2) * sizeof(char *));
	else
		n_argv = malloc(2 * sizeof(char *));
	return (n_argv);
}

t_cmd	*init_comand(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (0);
	cmd -> argv = NULL;
	cmd -> infile = 0;
	cmd -> outfile = 1;//para indicar salida estandard
	cmd -> heredoc = 0;
	cmd -> heredoc_delim = NULL;
	cmd -> next = NULL;
	return (cmd);
}
