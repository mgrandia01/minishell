/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 12:13:07 by arcmarti          #+#    #+#             */
/*   Updated: 2025/06/29 12:13:29 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_setup_signals(void);
void	ft_execute(t_cmd *cmds, char *envp[]);

// en los forks de las ejeciones, si son hijos hay que restarurar el estado
       	// de las senales
       	//if (pid == 0)
       	//{
    	//  signal(SIGINT, SIG_DFL);
	//    signal(SIGQUIT, SIG_DFL);
	//    ...
	//

void	ft_execute(t_cmd *cmds, char *envp[])
{
	pid_t pid;
	
	pid = fork();
	if (pid == 0)
	{
		
		if (cmds->outfile >= 3)
		{
			dup2(cmds->outfile, STDOUT_FILENO);
            		close(cmds->outfile);
            	}
            	ft_printf("\n");
            	for(int j=0; j<5; j++)
            	{
			if (!cmds->argv[j])
				break ;
			ft_printf("%s\t", cmds->argv[j]);
		}
		ft_printf("\n");
            	if (execve(cmds->argv[0], cmds->argv, envp) == -1)
		{
			printf("%s: %s %s \n", strerror(errno), cmds->argv[0], cmds->argv[1]); // valorar la posiblidad de customiziar ftprintf a la STDERR
			for(int j=0; j<5; j++)
			{
				if (!cmds->argv[j])
					break ;
				free (cmds->argv[j]);
			}
			free (cmds->argv);
			free (cmds);
			exit(1);
		}
				
	}
	else
	{
        	waitpid(pid, NULL, 0);
	}
}
