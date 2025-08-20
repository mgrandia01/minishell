/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:30:33 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/19 18:30:36 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_get_env_value(const char *key, t_list *env)
{
	t_env	*var;
	int		len;

	len = ft_strlen(key);
	while (env)
	{
		var = (t_env *)env->content;
		if (ft_strncmp(var->key, key, len) == 0)
			return (var->kval);
		env = env->next;
	}
	return (NULL);
}
