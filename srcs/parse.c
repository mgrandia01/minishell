/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/07 15:12:07 by mgrandia         ###   ########.fr       */
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

void add_word(t_cmd *cmd, char *word)
{
	int	size;
	char	**n_argv;
	char	*dup_word;

	dup_word = ft_strdup(word);
	if (!dup_word) //mensaje de error
		return ;
	size = argv_num(cmd->argv);
	if (cmd -> argv)
	{
		n_argv = ft_realloc(cmd -> argv, size * sizeof(char *), (size + 2) * sizeof(char *));
		if (!n_argv)
		{
			free(dup_word);
			return ;//mensaje de error?
		}
		cmd -> argv = n_argv;
	}
	else
	{
		cmd -> argv = malloc(2 * sizeof(char *));
		if (!cmd -> argv)
		{
			free(dup_word);
			return ;//mensaje de error
		}
	}
		
	cmd -> argv[size] = dup_word;
	cmd -> argv[size+1] = NULL;
	ft_printf("a size = %d, tenim = %s\n", size, cmd -> argv[size]);
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

struct s_cmd	*ft_parse(t_token *tokens)
{
	t_cmd	*cmd;
	
	cmd = init_comand();
	//cmds = NULL;
	while (tokens)
	{
		if ((tokens->type) == TOKEN_WORD)
			add_word(cmd, tokens->value);
		tokens = tokens->next;
	
	}
	return (cmd);
}


