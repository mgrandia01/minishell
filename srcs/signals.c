/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcmarti <arcmarti@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:03:55 by arcmarti          #+#    #+#             */
/*   Updated: 2025/08/30 17:11:26 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_handle_sigint(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	write(0, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
	g_exit_status = 130;
}

void	ft_handle_sigquit(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	display_new_line(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
}

void	ft_setup_signals(int enable)
{
	if (enable == 1)
	{
		signal(SIGINT, ft_handle_sigint);
		disable_sigquit_echo();
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
	}
}
