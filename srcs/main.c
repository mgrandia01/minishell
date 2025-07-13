/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 14:59:40 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/13 11:33:06 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        const char *type_str;
        const char *quote_str;

        switch (tokens->type)
        {
            case TOKEN_WORD:          type_str = "WORD"; break;
            case TOKEN_PIPE:          type_str = "PIPE"; break;
            case TOKEN_REDIR_IN:      type_str = "REDIR_IN"; break;
            case TOKEN_REDIR_OUT:     type_str = "REDIR_OUT"; break;
            case TOKEN_REDIR_APPEND:  type_str = "REDIR_APPEND"; break;
            case TOKEN_HEREDOC:       type_str = "HEREDOC"; break;
            case TOKEN_EOF:           type_str = "EOF"; break;
            default:                  type_str = "UNKNOWN"; break;
        }

        switch (tokens->quote)
        {
            case 0: quote_str = "NORMAL"; break;
            case 1: quote_str = "SINGLE_QUOTE"; break;
            case 2: quote_str = "DOUBLE_QUOTE"; break;
            default: quote_str = "UNKNOWN_QUOTE"; break;
        }

        printf("Token type: %s, value: '%s', quote: %s\n", type_str, tokens->value, quote_str);
        tokens = tokens->next;
    }
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*input = NULL;
	t_token	*tokens;
	t_cmd	*cmds, *cmd1;
	int	fd;
	//t_cmd	*cmds;

	(void) argc;
	(void) argv;
	// cambio el estado de las senyales que luego devolvere si hago forks
    	ft_setup_signals();
    	while (1)
    	{
        	input = readline("minishell \u25B8 ");
		// TEMPORAL MIENTRAS NO ESTA EL PARSING TEST 1
		ft_printf("\n TEST1.................");
		cmds = (t_cmd *)malloc(1 * sizeof(t_cmd));
		// Comando introducido por el usuario : ls -la
		// como en el pipex, el comando deberia de llevar el path correcto delante
		// String a splittear: "/usr/bin/ls -la"
		// Lista splitteada esperada: {"/usr/bin/ls", "-la", NULL}
		cmds->argv = ft_split("/usr/bin/ls -la", 32);       
		cmds->infile = -1;
		cmds->err_infile = 0;
		cmds->outfile = 1;
		cmds->err_outfile = 0;
		cmds->heredoc = -1;
		cmds->heredoc_delim = NULL;
		cmds->next = NULL;
		ft_execute(cmds, envp);
		for(int j=0; j<2; j++)
			free (cmds->argv[j]);
		free (cmds->argv);
		free (cmds);
		// FIN TEMPORAL MIENTRAS NO ESTA EL PARSING TEST 1
		// TEMPORAL MIENTRAS NO ESTA EL PARSING TEST 2
		ft_printf("\n TEST2.................");
		cmds = (t_cmd *)malloc(1 * sizeof(t_cmd));
		// Comando introducido por el usuario : grep -r --include="*.c" "main" .
		// Si van comillas dentro del comando, anyadir otra " delante para escaparlas
		// String a splittear: "/usr/bin/grep -r --include=""*.c"" ""main"" ."
		// Lista splitteada esperada: {"/usr/bin/grep", "-r", "--include=""*.c"", ""main"", ".", NULL}
		
		cmds->argv = ft_split("/usr/bin/grep -r --include=""*.c"" ""main"" .", 32); 
		cmds->infile = -1;
		cmds->err_infile = 0;
		cmds->outfile = 1;
		cmds->err_outfile = 0;
		cmds->heredoc = -1;
		cmds->heredoc_delim = NULL;
		cmds->next = NULL;
		ft_execute(cmds, envp);
		for(int j=0; j<5; j++)
			free (cmds->argv[j]);
		free (cmds->argv);
		free (cmds);
		// FIN TEMPORAL MIENTRAS NO ESTA EL PARSING TEST 2
		
		// TEMPORAL MIENTRAS NO ESTA EL PARSING TEST 3
		ft_printf("\n TEST3.................");
		cmd1 = (t_cmd *)malloc(1 * sizeof(t_cmd));
		fd = open("1.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
		// Comando introducido por el usuario : echo "pepito" > 1.txt
		// Si van comillas dentro del comando, anyadir otra " delante para escaparlas
		// String a splittear cmd1: "echo pepito" ........ qye oasa si va comillas???
		// String a splittear cmd2: > "1.txt"
		// Lista splitteada esperada: {"echo", "pepito", NULL} ->next-> {"1.txt", NULL}
		
		cmds = cmd1;
		cmd1->argv = ft_split("/usr/bin/echo pepito", 32); 
		cmd1->infile = -1;
		cmd1->err_infile = 0;
		cmd1->outfile = fd;
		cmd1->err_outfile = 0;
		cmd1->heredoc = -1;
		cmd1->heredoc_delim = NULL;
		ft_execute(cmds, envp);
		for(int j=0; j<2; j++)
			free (cmd1->argv[j]);
		free (cmd1->argv);
		free (cmd1);
		// FIN TEMPORAL MIENTRAS NO ESTA EL PARSING TEST 3
		
		if (!input)
			break;
		ft_add_history(input);
		//parser and store cmds
		tokens = ft_tokenize(input);
		print_tokens(tokens);
		free(input);
		if (!tokens)
			break ;
		// create list of nodes representing cmds
		cmds = ft_parse(tokens);
		free_tokens(tokens);
	//	free(tokens);
		if (!cmds)
			break ;
		// iterate list and execute cmds
		ft_execute(cmds, envp);
		(void)envp;
		ft_free_cmds(cmds);
       	}
	rl_clear_history();
       	return (0);
}
