/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 09:28:59 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/22 13:38:35 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_heredoc(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd)
	{
		while (i < cmd->heredoc_count)
		{
			free((cmd->heredocs[i]).delimiter);
			i++;
		}
		free(cmd->heredocs);
	}
}

void	sigint_handler_heredoc(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	g_exit_status = 130;
}

void	disable_sigquit(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	enable_sigquit(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	if (term.c_cc[VQUIT] == _POSIX_VDISABLE)
		term.c_cc[VQUIT] = 28;
	//term.c_cc[VQUIT] = '\\' & 0x1F;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	sigquit_handler_heredoc(int sig)
{
	(void)sig;
	//write(STDOUT_FILENO, "\n", 1);
	write(STDOUT_FILENO, "\b\b  \b\b", 6);
	printf("ssssss");fflush(0);
	disable_sigquit();
}
