/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 11:14:53 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/30 11:30:29 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	disable_sigquit_echo(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Función para inicializar el entorno y configuración inicial
t_list	*ft_init_minishell(char *envp[], int *exit_code)
{
	t_list	*l_env;

	ft_setup_signals(1);
	l_env = ft_init_env(envp);
	if (!l_env)
	{
		ft_putstr_fd("minishell: Error entorno no disponible\n", STDERR_FILENO);
		*exit_code = 1;
		return (NULL);
	}
	return (l_env);
}

// Función para manejar entrada del usuario
char	*ft_handle_input(void)
{
	char	*input;
	size_t	len;

	input = readline("\033[1;32mminishell \u25B8\033[0m ");
	if (!input || !ft_strncmp(input, "esc", 3))
	{
		g_exit_status = 0;
		ft_printf(STDOUT_FILENO, "exit\n");
		return (NULL);
	}
	if (*input)
		add_history(input);
	if (!ft_strncmp(input, "\n", 2))
	{
		free(input);
		return (ft_handle_input());
	}
	len = ft_strlen(input);
	if (len > 0 && input[len - 1] == '\n')
		input[len - 1] = '\0';
	return (input);
}

// Función para limpiar recursos al final
void	ft_cln_minishell(t_list **l_env, char *input, t_token *tokens)
{
	ft_lstclear(l_env, ft_free_env);
	if (input)
		free(input);
	if (tokens)
		free_tokens(tokens);
	rl_clear_history();
}
