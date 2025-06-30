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

static int	add_token(t_token **lst, t_token_type type, char *val, int quote)
{
	t_token	*new;

	new = malloc(sizeof(*new));
	if(!new || (type == TOKEN_WORD && !val))
		return (0);
  new->type = type;
  new->value = val;
  new->quote = quote;
  new->next = NULL;
  while(*lst && (*lst)->next)
    lst = &(*lst)->next;
  if(*lst)
    (*lst)->next = new;
  else
    *lst = new;
  return (1);
}

t_token	*ft_tokenize(char *input)
{
	int	state; // 0=normal, 1 = '', 2 = ""
	int	pos;
	int	start;
	t_token	*list;
	
	state = 0;
	pos = 0;
	start = 0;
	list = NULL;
	while (input[pos])
	{
		if (input[pos] == '\'' && state == 0)
			state = 1;
		else if (input[pos] == '\'' && state == 1)
			state = 0;
		else if (input[pos] == '"' && state == 0)
			state = 2;
		else if (input[pos] == '"' && state == 2)
			state = 0;
    else if (state == 0 && (input[pos] == ' ' || input[pos] == '\t'))
    {
      if (start != pos) //hay texto previo
        add_token(&list, TOKEN_WORD, ft_substr(input, start, pos - start), state);
      while (input[pos] == ' ' || input[pos] == '\t')
        pos ++;
      start = pos;
      continue;
    }
		pos ++;
	}
	return (list);
}

struct s_cmd	*ft_parse(t_token *tokens);


