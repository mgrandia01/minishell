/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 14:59:40 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/25 13:35:53 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

int	g_exit_status = 0;

void	disable_sigquit_echo(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
/*
void	print_tokens(t_token *tokens)
{
	const char *type_str;
	while (tokens)
	{
	switch (tokens->type)
	{
		case TOKEN_WORD:		type_str = "WORD"; break;
		case TOKEN_PIPE:		type_str = "PIPE"; break;
		case TOKEN_REDIR_IN:		type_str = "REDIR_IN"; break;
		case TOKEN_REDIR_OUT:		type_str = "REDIR_OUT"; break;
		case TOKEN_REDIR_APPEND:	type_str = "REDIR_APPEND"; break;
		case TOKEN_HEREDOC:		type_str = "HEREDOC"; break;
		case TOKEN_EOF:			type_str = "EOF"; break;
		default:			type_str = "UNKNOWN"; break;
	}
	printf("Token type: %s, value: '%s', end: %d\n", type_str, tokens->value, tokens->end);
	tokens = tokens->next;
	}
}
*/
/*
// Función para inicializar el entorno y configuración inicial
static t_list	*ft_init_minishell(char *envp[], int *exit_code)
{
	t_list	*l_env;

	ft_setup_signals(1);
	disable_sigquit_echo();
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
static char	*ft_handle_input(void)
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

// Función para procesar tokens y validar heredocs
static int	ft_process_tokens(t_token **tokens, char *input)
{
	*tokens = ft_tokenize(input, 0);
	free(input);
	if (!*tokens)
		return (0);
	if (ft_count_heredocs(*tokens) > 16)
	{
		ft_printf(STDERR_FILENO, "minishell: maximum here-document count exceeded\n");
		return (0);
	}
	return (1);
}

// Función para expandir tokens y preparar entorno
static int	ft_expand_and_prepare(t_token **tokens, t_list *l_env)
{
	char	**envp_exec;

	envp_exec = ft_build_envp_array(l_env);
	if (!envp_exec)
	{
		free_tokens(*tokens);
		*tokens = NULL;
		ft_printf(STDERR_FILENO, "Error: falló la creación de envp\n");
		return (0);
	}
	process_token_expansion(tokens, envp_exec, 0);
	ft_free_tab(envp_exec);
	join_tokens_with_end(tokens);
	remove_quotes_from_token_list(*tokens);
	return (1);
}

// Función para parsear y ejecutar comandos
static void	ft_parse_and_execute(t_token *tokens, t_list *l_env)
{
	t_cmd	*cmds;

	cmds = ft_parse(tokens);
	free_tokens(tokens);
	if (!cmds)
	{
		ft_printf(STDERR_FILENO, "Error: falló el parseo\n");
		return ;
	}
	ft_exe_pipeline(cmds, l_env);
	ft_cmdclear(&cmds, ft_free_argv);
}

// Función principal del loop
static void	ft_main_loop(t_list *l_env)
{
	char	*input;
	t_token	*tokens;

	while (1)
	{
		input = ft_handle_input();
		if (!input)
			break ;
		if (!ft_process_tokens(&tokens, input))
			break ;
		if (!ft_expand_and_prepare(&tokens, l_env))
			continue ;
		ft_parse_and_execute(tokens, l_env);
	}
}

// Función para limpiar recursos al final
static void	ft_cleanup_minishell(t_list **l_env, char *input, t_token *tokens)
{
	ft_lstclear(l_env, ft_free_env);
	if (input)
		free(input);
	if (tokens)
		free_tokens(tokens);
	rl_clear_history();
}*/

// Función main refactorizada
/*int	main(int argc, char *argv[], char *envp[])
{
	char	*input;
	t_token	*tokens;
	t_list	*l_env;
	int		exit_code;

	(void) argc;
	(void) argv;
	input = NULL;
	tokens = NULL;
	exit_code = 0;
	l_env = ft_init_minishell(envp, &exit_code);
	if (!l_env)
		return (exit_code);
	ft_main_loop(l_env);
	ft_cleanup_minishell(&l_env, input, tokens);
	return (0);
}*/
/* readline sin normi
int	main(int argc, char *argv[], char *envp[])
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmds;
	size_t	len;
	t_list	*l_env;
	char	**envp_exec;

	(void) argc;
	(void) argv;
	ft_setup_signals(1);
	input = NULL;
	tokens = NULL;
	cmds = NULL;
	l_env = ft_init_env(envp);
	if (!l_env)
	{
		ft_putstr_fd("minishell: Error entorno no disponible\n", STDERR_FILENO);
		return (1);
	}
	envp_exec = NULL;
	disable_sigquit_echo();
	while (1)
	{
		input = readline("\033[1;32mminishell \u25B8\033[0m ");
		if (!input || !ft_strncmp(input, "esc", 3))
		{
			g_exit_status = 0;
			ft_printf(STDOUT_FILENO, "exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		if (!ft_strncmp(input, "\n", 2))
		{
			free(input);
			continue ;
		}
		len = ft_strlen(input);
		if (len > 0 && input[len - 1] == '\n')
			input[len - 1] = '\0';
		tokens = ft_tokenize(input, 0);
		free(input);
		input = NULL;
		if (!tokens)
			break ;
		print_tokens(tokens);
		if (ft_count_heredocs(tokens) > 16)
		{
			ft_printf(STDERR_FILENO, "minishell: maximum here-document count exceeded\n");
			break ;
		}
		envp_exec = ft_build_envp_array(l_env);
		if (!envp_exec)
		{
			free_tokens(tokens);
			tokens = NULL;
			ft_printf(STDERR_FILENO, "Error: falló la creación de envp\n");
			continue ;
		}
		process_token_expansion(&tokens, envp_exec, 0);
		ft_free_tab(envp_exec);
		join_tokens_with_end(&tokens);
		remove_quotes_from_token_list(tokens);
		cmds = ft_parse(tokens);
		free_tokens(tokens);
		tokens = NULL;
		if (!cmds)
		{
			ft_printf(STDERR_FILENO, "Error: falló el parseo\n");
			continue ;
		}
		ft_exe_pipeline(cmds, l_env);
		ft_cmdclear (&cmds, ft_free_argv);
	}
	ft_lstclear(&l_env, ft_free_env);
	if (input)
		free(input);
	if (tokens)
		free_tokens(tokens);
	rl_clear_history();
	return (0);
}
*/

//main gnl
int	main(int argc, char *argv[], char *envp[])
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmds;
	size_t	len;
	t_list	*l_env;
	char	**envp_exec;

	(void) argc;
	(void) argv;
	ft_setup_signals(1);
	input = NULL;
	tokens = NULL;
	cmds = NULL;
	l_env = ft_init_env(envp);
	if (!l_env)
	{
		ft_putstr_fd("minishell: Error entorno no disponible\n", STDERR_FILENO);
		return (1);
	}
	envp_exec = NULL;
	//disable_sigquit_echo();
	while (1)
	{
		if (isatty(STDIN_FILENO))
			ft_printf(STDOUT_FILENO, "\033[1;32mminishell \u25B8\033[0m ");
		input = get_next_line(STDIN_FILENO);
		if (!input || !ft_strncmp(input, "esc", 3))
		{
			g_exit_status = 0;
			ft_printf(STDOUT_FILENO, "exit\n");
			break ;
		}
		if (!ft_strncmp(input, "\n", 2))
		{
			free(input);
			continue ;
		}
		len = ft_strlen(input);
		if (len > 0 && input[len - 1] == '\n')
			input[len - 1] = '\0';
		tokens = ft_tokenize(input, 0);
		free(input);
		input = NULL;
		if (!tokens)
			break ;
		if (ft_count_heredocs(tokens) > 16)
		{
			ft_printf(STDERR_FILENO, "minishell: maximum here-document count exceeded\n");
			break ;
		}
		envp_exec = ft_build_envp_array(l_env);
		if (!envp_exec)
		{
			free_tokens(tokens);
			tokens = NULL;
			ft_printf(STDERR_FILENO, "Error: falló la creación de envp\n");
			continue ;
		}
		process_token_expansion(&tokens, envp_exec, 0);
		ft_free_tab(envp_exec);
		join_tokens_with_end(&tokens);
		remove_quotes_from_token_list(tokens);
		cmds = ft_parse(tokens);
		free_tokens(tokens);
		tokens = NULL;
		if (!cmds)
		{
			ft_printf(STDERR_FILENO, "Error: falló el parseo\n");
			continue ;
		}
		ft_exe_pipeline(cmds, l_env);
		ft_cmdclear (&cmds, ft_free_argv);
	}
	ft_lstclear(&l_env, ft_free_env);
	if (input)
		free(input);
	if (tokens)
		free_tokens(tokens);
	rl_clear_history();
	return (0);
}

