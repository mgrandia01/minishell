/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 13:25:55 by arcmarti          #+#    #+#             */
/*   Updated: 2025/07/07 11:42:19 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_env(void *content)
{
	t_env *var = (t_env *)content;
	if (var)
	{
		free(var->key);
		free(var->kval);
		free(var);
	}
}

t_list	*ft_init_env(char **envp)
{
	t_list	*env_list;
	t_env	*var;
	int	i;
	char	*equal;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			var = (t_env *)malloc(sizeof(t_env));
		        if (!var)
		        {
		            ft_lstclear(&env_list, ft_free_env);
		            return (NULL);
		        }
		        var->key = ft_substr(envp[i], 0, equal - envp[i]);
		        var->kval = ft_strdup(equal + 1);
        		var->kexp = 1;
			ft_lstadd_back(&env_list, ft_lstnew(var)); 
	       	}
        	i++;
	}
	return env_list;
}


int	ft_is_numeric(const char *str)
{
	int	i;

	if (!str || !str[0])
        	return (0);
	i = 0;
	if (str[0] == '+' || str[0] == '-')
	        i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (i > (str[0] == '+' || str[0] == '-'));
}
void	ft_free_cmds(t_cmd *cmds)
{
	(void)	cmds;
}

void	ft_setup_signals()
{

}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

/*t_token	*ft_tokenize(char *input)
{
	(void) input;
	
	return (NULL);
}*/

/*
struct s_cmd	*ft_parse(t_token *tokens)
{
	t_cmd	*cmds;
	
	(void) tokens;
	cmds = NULL;
	return (cmds);
}*/


