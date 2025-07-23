/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/14 16:28:48 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	find_var(char	*value)
{
	int	i;
	
	i = 0;
	if (!value)
		return (0);
	while(value[i])
	{
		if (value[i] == '$')
			return (1);
		i++;	
	}
	return (0);
}

void	process_expansion(t_token **tokens,  char *envp[])
{
	t_token	*new_list;
	t_token	*current;
	int	i;
	int	quote;

	new_list = NULL;
	current = *tokens;
	queto = 0;// 0= no quote, 1= '', 2= ""
	(void)envp;
	while (current != NULL)
	{
		//quote = ft_quoted(current->value[0]);
		i = 0;
		while (current->value[i] != '\0')
		{
			if (current->value[i] == '$')
				//if (quoted == 2) no expandir res		
			//aqui analizas cada caracter de el token
			i++;
		}
		current = current->next;
	}
	//free_token(*tokens);
	*tokens = new_list;
}
