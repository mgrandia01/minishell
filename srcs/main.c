/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 14:59:40 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/02 12:06:19 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char *argv[], char *envp[])
{
	char	*input = NULL;
	t_token	*tokens;
	t_cmd	*cmds;

	(void) argc;
	(void) argv;
	// cambio el estado de las senyales que luego devolvere si hago forks
    	ft_setup_signals();
    	while (1)
    	{
        	input = readline("minishell \u25B8 ");
		/*
		// TEMPORAL MIENTRAS NO ESTA EL PARSING TEST 1
		ft_printf("TEST1.................");
		cmds = (t_cmd *)malloc(1 * sizeof(t_cmd));
		// comando forzado : ls -la
		// Lista splitteada esperada: {"/usr/bin/ls", "-la", NULL}
		cmds->argv = ft_split("/usr/bin/ls -la", 32);       
		cmds->infile = -1;
		cmds->outfile = -1;
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
		ft_printf("TEST2.................");
		cmds = (t_cmd *)malloc(1 * sizeof(t_cmd));
		// comando forzado : grep -r --include="*.c" "main" .
		// Lista splitteada esperada: {"/usr/bin/grep", "-r", "--include="*.c"", ""main"", ".", NULL}
		cmds->argv = ft_split("/usr/bin/grep -r --include=\"*.c\" \"main\" .", 32); 
		cmds->infile = -1;
		cmds->outfile = -1;
		cmds->heredoc = -1;
		cmds->heredoc_delim = NULL;
		cmds->next = NULL;
		ft_execute(cmds, envp);
		for(int j=0; j<5; j++)
			free (cmds->argv[j]);
		free (cmds->argv);
		free (cmds);
		// FIN TEMPORAL MIENTRAS NO ESTA EL PARSING TEST 2
		*/
		if (!input)
			break;
		ft_add_history(input);
		//parser and store cmds
		tokens = ft_tokenize(input);
		free(input);
		if (!tokens)
			break ;
		// create list of nodes representing cmds
		cmds = ft_parse(tokens);
		free(tokens);
		if (!cmds)
			break ;
		// iterate list and execute cmds
		ft_execute(cmds, envp);
		ft_free_cmds(cmds);
       	}
	rl_clear_history();
       	return (0);
}
