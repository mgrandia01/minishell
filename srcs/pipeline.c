/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:08:16 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/25 11:36:25 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// function to free memomry if error creating pipeline or childs processes
void	ft_free_mem_exe(t_cmd *cmd, t_list *l_env, int **pline, int ex_v)
{
	perror("minishell: error executing pipeline");
	g_exit_status = ex_v;
	ft_free_pipeline(pline);
	ft_cmdclear (&cmd, ft_free_argv);
	ft_lstclear(&l_env, ft_free_env);
	exit (g_exit_status);
}

// free all pipeline closing only opened pipes
void	ft_free_pipeline(int **pipeline)
{
	int	i;

	if (!pipeline)
		return ;
	i = 0;
	while (pipeline[i] != NULL)
	{
		if (pipeline[i][0] >= 0)
		{
			close(pipeline[i][0]);
			pipeline[i][0] = -1;
		}
		if (pipeline[i][1] >= 0)
		{
			close(pipeline[i][1]);
			pipeline[i][1] = -1;
		}
		free(pipeline[i]);
		i++;
	}
	free(pipeline);
}

// pipeline is a **int null-finished containing int[2] to store pipes
// pipes are opened
int	ft_create_pipes(int ***pipeline, int n_pipes)
{
	int	i;

	*pipeline = (int **) malloc ((n_pipes + 1) * 2 * sizeof(int *));
	if (!*pipeline)
		return (0);
	i = 0;
	while (i < n_pipes)
	{
		(*pipeline)[i] = (int *) malloc (2 * sizeof(int));
		if (!(*pipeline)[i])
			return (0);
		i++;
	}
	(*pipeline)[i] = NULL;
	i = 0;
	while (i < n_pipes)
	{
		if (pipe((*pipeline)[i]) == -1)
			return (0);
		i++;
	}
	return (1);
}

// n_pipes is n_procs - 1
int	ft_calculate_pipes(t_cmd *cmd)
{
	int		n_pipes;
	t_cmd	*current;

	n_pipes = 0;
	current = cmd;
	while (current)
	{
		n_pipes++;
		current = current->next;
	}
	return (n_pipes - 1);
}

// function to close pipes of the pipeline
void	ft_close_pipes(int **pipeline, int n_pipes)
{
	int	i;

	i = 0;
	while (i < n_pipes)
	{
		if (pipeline[i][0])
		{
			close(pipeline[i][0]);
			pipeline[i][0] = -1;
		}
		if (pipeline[i][1])
		{
			close(pipeline[i][1]);
			pipeline[i][1] = -1;
		}
		i++;
	}
}
