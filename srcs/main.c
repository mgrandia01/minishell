/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 14:59:40 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/30 11:39:39 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
//#include <stdio.h>

int	g_exit_status = 0;

// Función para procesar tokens y validar heredocs
static int	ft_process_tokens(t_token **tokens, char *input)
{
	*tokens = ft_tokenize(input, 0);
	free(input);
	if (!*tokens)
		return (0);
	if (ft_count_heredocs(*tokens) > 16)
	{
		ft_printf(STDERR_FILENO, "minishell: ");
		ft_printf(STDERR_FILENO, "maximum here-document count exceeded\n");
		free_tokens(*tokens);
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
	process_token_exp(tokens, envp_exec, 0);
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
	tokens = NULL;
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

// Función main refactorizada
int	main(int argc, char *argv[], char *envp[])
{
	char	*input;
	t_token	*tokens;
	t_list	*l_env;
	int		exit_code;

	if (argc != 1)
	{
		ft_printf(STDERR_FILENO, "Error: more arguments than allowed\n");
		return (0);
	}
	(void) argv;
	input = NULL;
	tokens = NULL;
	exit_code = 0;
	l_env = ft_init_minishell(envp, &exit_code);
	if (!l_env)
		return (exit_code);
	ft_main_loop(l_env);
	ft_cln_minishell(&l_env, input, tokens);
	return (0);
}
