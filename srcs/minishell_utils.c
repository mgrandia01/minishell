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

int	ft_is_valid_key(const char *key)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	*ft_strjoin_triple(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(s1, s2);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

char	**ft_build_envp_array(t_list *l_env)
{
	int		count;
	int		i;
	char	**envp;
	t_env	*var;

	i = 0;
	envp = NULL;
	count = ft_lstsize(l_env);
	envp = (char **) malloc(sizeof(char *) * (count + 1));
	if (!l_env)
		return (NULL);
	while (l_env)
	{
		var = (t_env *)l_env->content;
		if (var->kexp && var->key && var->kval)
		{
			envp[i] = ft_strjoin_triple(var->key, "=", var->kval);
			i++;
		}
		l_env = l_env->next;
	}
	envp[i] = NULL;
	return (envp);
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

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
