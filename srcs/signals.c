/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:03:55 by arcmarti          #+#    #+#             */
/*   Updated: 2025/07/15 14:03:57 by arcmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	//rl_on_new_line();		// resetea línea interna de readline
	//rl_replace_line("", 0);		// limpia línea actual
	//rl_redisplay();			// vuelve a mostrar el prompt
	if (isatty(STDIN_FILENO))
        	ft_printf(STDOUT_FILENO,"\033[1;32mminishell \u25B8\033[0m ");
}

void	ft_handle_sigquit(int sig)
{
	(void)sig;
	//rl_on_new_line();                        // Resetea estado interno de readline
	//rl_redisplay();                          // Vuelve a mostrar el prompt
	write(STDOUT_FILENO, "\b\b  \b\b", 6);  // Borra visualmente dos caracteres
	
	// cuidado con usar tgetstr(), tgoto() y tputs() pq pueden interferir con readline
}

void	ft_setup_signals(void)
{
    signal(SIGINT, ft_handle_sigint);	// Ctrl+C
    signal(SIGQUIT, ft_handle_sigquit);		// Ctrl+ contrabarra
}
