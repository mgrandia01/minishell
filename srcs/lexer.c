/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:15:18 by mgrandia          #+#    #+#             */
/*   Updated: 2025/06/28 16:15:20 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_add_history(char *input);
t_token	*ft_tokenize(char *input);
struct s_cmd	*ft_parse(t_token *tokens);

/*
char  *read_input(void)
{
  char  *input;

  input = readline("minishell> ");
  if (input && )

}*/
