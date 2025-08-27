/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/27 17:03:47 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* 
 * Searches for the value of an environment variable by name.
 * Returns a pointer to the value part (after the '=') if found in envp.
 * Returns NULL if the variable is not found.
 */
static char	*get_env_value(const char *name, char *envp[])
{
	int		i;
	size_t	len;

	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i ++;
	}
	return (NULL);
}

/*
 * Expands an environment variable starting at index *i in t_val.
 * Looks up its value in envp and returns a duplicated string of the value.
 * Advances *i past the variable name.
 */
/*
void    p_tokens(t_token *tokens)
{
        const char *type_str;
        while (tokens)
        {
        switch (tokens->type)
        {
                case TOKEN_WORD:                type_str = "WORD"; break;
                case TOKEN_PIPE:                type_str = "PIPE"; break;
                case TOKEN_REDIR_IN:            type_str = "REDIR_IN"; break;
                case TOKEN_REDIR_OUT:           type_str = "REDIR_OUT"; break;
                case TOKEN_REDIR_APPEND:        type_str = "REDIR_APPEND"; break;
                case TOKEN_HEREDOC:             type_str = "HEREDOC"; break;
                case TOKEN_EOF:                 type_str = "EOF"; break;
                default:                        type_str = "UNKNOWN"; break;
        }
        printf("Token type: %s, value: '%s', end: %d\n", type_str, tokens->value, tokens->end);
        tokens = tokens->next;
        }
}*/

char	*exp_var_at_index(const char *t_val, int *i, char *envp[], t_token *c)
{
	char	*var_name;
	char	*value;
	int		start;
	int		len;

	start = *i;
	(*i)++;
	len = 0;
	while (t_val[*i + len] && (ft_isalnum(t_val[*i + len])
			|| t_val[*i + len] == '_'))
		len++;
	var_name = ft_substr(t_val, start + 1, len);
	*i += len;
	(void) c;
//	printf("\nt_val[*(i)] = %c", t_val[*(i)]);
//	printf("\nt_val[*(i+1)] = %c", t_val[*i+1]);
//	if (t_val[*i] == '$' && t_val[*(i+1)] != '\0') 
//		c->end = 1;

//	p_tokens(c);
	if (!var_name)
		return (NULL);
	value = get_env_value(var_name, envp);
	free(var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}
