/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 13:25:55 by arcmarti          #+#    #+#             */
/*   Updated: 2025/06/29 13:25:56 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_cmds(t_cmd *cmds)
{
	(void)	cmds;
}

void	ft_add_history(char *input)
{
	(void)	input;
}

void	ft_setup_signals()
{

}

/*t_token	*ft_tokenize(char *input)
{
	(void) input;
	
	return (NULL);
}*/

struct s_cmd	*ft_parse(t_token *tokens)
{
	t_cmd	*cmds;
	
	(void) tokens;
	cmds = NULL;
	return (cmds);
}


