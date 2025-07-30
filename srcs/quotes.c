/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/26 15:27:44 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*remove_quotes(char *str)
{
	size_t	len;
	char	quote;

	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'')
			|| (str[0] == '"' && str[len - 1] == '"')))
	{
		quote = str[0];
		if (str[len - 1] == quote)
			return (ft_substr(str, 1, len - 2));
	}
	return (ft_strdup(str));
}

void remove_quotes_from_token_list(t_token *token)
{
	char *cleaned;

	while (token)
	{
		cleaned = remove_quotes(token->value);
		free(token->value);
		token->value = cleaned;
		token = token->next;
	}
}

