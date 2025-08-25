/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 14:59:40 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/25 11:48:23 by mgrandia         ###   ########.fr       */
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

/*
//main gnl
int	maini(int argc, char *argv[], char *envp[])
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
		tokens = ft_tokenize(input);
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
		process_token_expansion(&tokens, envp_exec);
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
