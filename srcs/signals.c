/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:03:55 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/28 11:23:22 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/*
void	cleanup_outfiles(t_cmd *cmd)
{
	int i;

	if (!cmd || !cmd->outfile_name)
		return;

	for (i = 0; i < cmd->outfile_count; i++)
		unlink(cmd->outfile_name[i]); // borra el archivo del sistema
}
*/


void	ft_handle_sigint(int sig)
{
//printf("SIGNALS");
	(void)sig;
	rl_replace_line("", 0);
	write(0, "\n", 1);
	//rl_replace_line(" ", STDOUT_FILENO);
	rl_on_new_line();		// resetea línea interna de readline
	//rl_replace_line("CTRL C", 0);		// limpia línea actual
	rl_redisplay();			// vuelve a mostrar el prompt
	//if (isatty(STDIN_FILENO))
	//	ft_printf(STDOUT_FILENO, "\033[1;32mminishell \u25B8\033[0m ");
	g_exit_status = 130;
}

void	ft_handle_sigquit(int sig)
{
	
	(void)sig;
	rl_on_new_line();                        // Resetea estado interno de readline
	write(0, "\n", 1);
	rl_replace_line("", 0);
	rl_redisplay();                          // Vuelve a mostrar el prompt	


	//write(STDOUT_FILENO, "\b\b  \b\b", 6);// Borra visualmente dos caracteres
	// cuidado con usar tgetstr(), tgoto() y tputs() pq pueden interferir con readline
}

void	display_new_line(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
}

// SIGINT CTRL + C
// SIGQUIT CTRL + contrabrarra
void	ft_setup_signals(int enable)
{
	if (enable ==  1)
	{
		signal(SIGINT, ft_handle_sigint);
		signal(SIGQUIT, ft_handle_sigquit);
	}
	else if (enable == 2)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else
	{
	signal(SIGINT, display_new_line);
	signal(SIGQUIT, display_new_line);
		//signal(SIGINT, SIG_IGN);
		//signal(SIGQUIT, SIG_IGN);
	}
}
