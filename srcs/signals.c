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

void    handle_sigint(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();           // resetea línea interna de readline
    rl_replace_line("", 0);     // limpia línea actual
    rl_redisplay();             // vuelve a mostrar el prompt
}


void    setup_signals(void)
{
    signal(SIGINT, handle_sigint);  // Ctrl+C
    signal(SIGQUIT, SIG_IGN);       // Ctrl+\
}
