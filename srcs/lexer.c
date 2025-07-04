/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:15:18 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/04 12:38:20 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_add_history(char *input);

static int	add_token(t_token **lst, t_token_type type, char *val, int quote)
{
	t_token	*new;

	new = malloc(sizeof(*new));
	if (!new || (type == TOKEN_WORD && !val))
		return (0);
	new->type = type;
	new->value = val;
	new->quote = quote;
	new->next = NULL;
	while (*lst && (*lst)->next)
		lst = &(*lst)->next;
	if (*lst)
		(*lst)->next = new;
	else
		*lst = new;
	return (1);
}

static int	ft_get_state(char input, int state)
{
	if (input == '\'' && state == 0)
		state = 1;
	else if (input == '\'' && state == 1)
		state = 0;
	else if (input == '"' && state == 0)
		state = 2;
	else if (input == '"' && state == 2)
		state = 0;
	return (state);
}

//state : 0=normal, 1 = '', 2 = ""
t_token	*ft_tokenize(char *input)
{
	int		state;
	int		pos;
	int		start;
	t_token	*list;
	char	*val;
	char	quote;

	state = 0;
	pos = 0;
	start = 0;
	list = NULL;
	val = NULL;
	quote = input[pos];
	while (input[pos])
	{
		if ((input[pos] == '\'') || (input[pos] == '\"'))
		{
			//en bash "hola""mundo" = holamundo?
			state = ft_get_state(input[pos], state);
			quote = input[pos];
			pos ++;
			start = pos;
			while(input[pos] && input[pos] != quote)
				pos ++;
			if (!input[pos])
			{

				//TODO Gestionar que pasa si no se cierran las comillas
				ft_printf("no se han cerrado las comillas");
				free(val);
				return (NULL);
			}
			val = ft_substr(input, start, pos - start);
			add_token(&list, TOKEN_WORD, val, state);
			if (input[pos] == quote)
			{
				state = ft_get_state(input[pos], state);
				pos ++;
			}
			start = pos;
			continue ;

		}
		if (state == 0 && (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\0'))
		{
			val = ft_substr(input, start, pos - start);
			if (start != pos) //hay texto previo FIXME
				add_token(&list, TOKEN_WORD, val, state);
			while (input[pos] == ' ' || input[pos] == '\t')
				pos ++;
			start = pos;
			continue ;
		}
		
		else if (input[pos] == '>') //i si tenemos >>>?
		{
			if (start != pos) //hay texto previo FIXME
			{	
				val = ft_substr(input, start, pos - start);
				add_token(&list, TOKEN_WORD, val, state);
			}

			if (input[pos+1] == '>')
			{
				add_token(&list, TOKEN_REDIR_APPEND, ft_strdup(">>"), state);
				pos = pos + 2;
			}
			else
			{
				add_token(&list, TOKEN_REDIR_OUT, ft_strdup(">"), state);
				pos ++;
			}
			start = pos;
		}
		else if (input[pos] == '<')
		{
			if (start != pos) //hay texto previo FIXME
			{	
				val = ft_substr(input, start, pos - start);
				add_token(&list, TOKEN_WORD, val, state);
			}

			if (input[pos+1] == '<')
			{
				add_token(&list, TOKEN_HEREDOC, ft_strdup("<<"), state);
				pos = pos + 2;
			}
			else
			{
				add_token(&list, TOKEN_REDIR_IN, ft_strdup("<"), state);
				pos ++;
			}
			start = pos;
		}
		else if (input[pos] == '|')
		{
			if (start != pos) //hay texto previo FIXME
			{	
				val = ft_substr(input, start, pos - start);
				add_token(&list, TOKEN_WORD, val, state);
			}

				add_token(&list, TOKEN_PIPE, ft_strdup("|"), state);
				pos ++;
				start = pos;
		}

		else
			pos ++;
	}
	if (start != pos) //hay texto previo FIXME
	{	
		val = ft_substr(input, start, pos - start);
		add_token(&list, TOKEN_WORD, val, state);
	}
	//free(val)?
	add_token(&list, TOKEN_EOF, "EOF", 0);
	return (list);
}

struct s_cmd	*ft_parse(t_token *tokens);
