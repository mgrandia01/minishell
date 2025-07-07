/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/07 12:16:48 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void add_word(t_cmd *cmd, char *word)
{
	cmd -> argv[0] = word;
	
	//ft_printf("%s", word);

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
	ft_printf("hola");
	return (cmd);
}


