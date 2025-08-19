/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 14:59:40 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/02 13:18:21 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

int g_exit_status = 0;

void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        const char *type_str;
 //       const char *quote_str;

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

/*        switch (tokens->quote)
        {
            case 0: quote_str = "NORMAL"; break;
            case 1: quote_str = "SINGLE_QUOTE"; break;
            case 2: quote_str = "DOUBLE_QUOTE"; break;
            default: quote_str = "UNKNOWN_QUOTE"; break;
        }
*/
        printf("Token type: %s, value: '%s', end: %d\n", type_str, tokens->value, tokens->end);
        //printf("Token type: %s, value: '%s', quote: %s, end: %d\n", type_str, tokens->value, quote_str, tokens->end);
        tokens = tokens->next;
    }
}

void	print_commands(t_cmd *cmd_list)
{
	int i;
	int cmd_num = 1;

	while (cmd_list)
	{
		printf("======= Comando %d =======\n", cmd_num);

		// Imprimir argumentos
		if (cmd_list->argv)
		{
			i = 0;
			while (cmd_list->argv[i])
			{
				printf("argv[%d]: %s\n", i, cmd_list->argv[i]);
				i++;
			}
		}
		else
			printf("argv: (null)\n");

		// Redirecciones
		printf("infile: %d\n", cmd_list->infile);
		printf("outfile: %d\n", cmd_list->outfile);

		// Heredoc
		if (cmd_list->heredoc_count > 0 && cmd_list->heredocs)
		{
		        printf("heredocs: %d delimitador(es):\n", cmd_list->heredoc_count);
		        for (i = 0; i < cmd_list->heredoc_count; i++)
		        {
        		    printf("  heredoc[%d]: \"%s\"\n", i, cmd_list->heredocs[i].delimiter);
      			  }
    		}
   		 else
  		  {
  		      printf("heredoc: no\n");
 	}
		printf("\n");
		cmd_list = cmd_list->next;
		cmd_num++;
	}
}



/*void ft_exe_tests(t_cmd *cmd_ignore, char *envp[])
{
	t_cmd	*cmds, *cmd1;
	int	fd;
	
	(void)envp;
	(void) *cmd_ignore;
	// TEMPORAL MIENTRAS NO ESTA EL PARSING TEST 1
	ft_printf(STDOUT_FILENO,"\n TEST1.................");
	cmds = (t_cmd *)malloc(1 * sizeof(t_cmd));
	// Comando introducido por el usuario : ls -la
	// como en el pipex, el comando deberia de llevar el path correcto delante
	// String a splittear: "/usr/bin/ls -la"
	// Lista splitteada esperada: {"/usr/bin/ls", "-la", NULL}
	cmds->argv = ft_split("/usr/bin/ls -la", 32);       
	cmds->infile = -1;
//	cmds->err_infile = 0;
	cmds->outfile = 1;
//	cmds->err_outfile = 0;
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
	ft_printf(STDOUT_FILENO, "\n TEST2.................");
	cmds = (t_cmd *)malloc(1 * sizeof(t_cmd));
	// Comando introducido por el usuario : grep -r --include="*.c" "main" .
	// Si van comillas dentro del comando, anyadir otra " delante para escaparlas
	// String a splittear: "/usr/bin/grep -r --include=""*.c"" ""main"" ."
	// Lista splitteada esperada: {"/usr/bin/grep", "-r", "--include=""*.c"", ""main"", ".", NULL}
	
	cmds->argv = ft_split("/usr/bin/grep -r --include=""*.c"" ""main"" .", 32); 
	cmds->infile = -1;
//	cmds->err_infile = 0;
	cmds->outfile = 1;
//	cmds->err_outfile = 0;
	cmds->heredoc = -1;
	cmds->heredoc_delim = NULL;
	cmds->next = NULL;
	//ft_exe_pipeline(cmds, envp);
	for(int j=0; j<5; j++)
		free (cmds->argv[j]);
	free (cmds->argv);
	free (cmds);
	// FIN TEMPORAL MIENTRAS NO ESTA EL PARSING TEST 2
	
	// TEMPORAL MIENTRAS NO ESTA EL PARSING TEST 3
	ft_printf(STDOUT_FILENO, "\n TEST3.................");
	cmd1 = (t_cmd *)malloc(1 * sizeof(t_cmd));
	fd = open("1.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
	// Comando introducido por el usuario : echo "pepito" > 1.txt
	// Si van comillas dentro del comando, anyadir otra " delante para escaparlas
	// String a splittear cmd1: "echo pepito" ........ qye oasa si va comillas???
	// String a splittear cmd2: > "1.txt"
	// Lista splitteada esperada: {"/usr/bin/echo", "pepito", NULL} ->next-> {"1.txt", NULL}
	
	cmds = cmd1;
	cmd1->argv = ft_split("/usr/bin/echo pepito", 32); 
	cmd1->infile = -1;
//	cmd1->err_infile = 0;
	cmd1->outfile = fd;
//	cmd1->err_outfile = 0;
	cmd1->heredoc = -1;
	cmd1->heredoc_delim = NULL;
	ft_execute(cmds, envp);
	for(int j=0; j<2; j++)
		free (cmd1->argv[j]);
	free (cmd1->argv);
	free (cmd1);
	// FIN TEMPORAL MIENTRAS NO ESTA EL PARSING TEST 3
}
*/

int	main(int argc, char *argv[], char *envp[])
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmds;
	//char	*path;
	size_t len;
	t_list *l_env;
	char    **envp_exec;
       
	(void) argc;
	(void) argv;
	// cambio el estado de las senyales que luego devolvere si hago forks
    	ft_setup_signals(1);
    	input = NULL;
    	tokens = NULL;
    	cmds = NULL;
    	l_env = ft_init_env(envp);
	envp_exec = NULL;
    	/*t_list *env_list2 = l_env;
    	while (env_list2)
	{
        	t_env *var = (t_env *)env_list2->content;
 	       printf("key=%s, value=%s, exported=%d\n", var->key, var->kval, var->kexp);
 	       env_list2 = env_list2->next;
    	}*/
	while (1)
    	{
        	//ft_printf("minishell \u25B8 ");
        	//if (isatty(STDIN_FILENO))
        	//	ft_printf(STDOUT_FILENO,"\033[1;32mminishell \u25B8\033[0m ");
        	input = get_next_line(STDIN_FILENO);
        	//input = readline("\033[1;32mminishell \u25B8\033[0m ");//FIXME historial
		//("\nminishell \u25B8 "); //para las senales y el history ira bien

		if (!input || !ft_strncmp(input,"esc",3))
		{
			g_exit_status = 0;
			ft_printf(STDOUT_FILENO,"exit\n");
			break;
		}
		//if (*input)
		//	add_history(input); //FIXME historial
		if (!ft_strncmp(input,"\n",2))
		{
			free(input);
			continue ;
		}
		len = ft_strlen(input);
		if (len> 0 && input[len - 1] == '\n')
			input[len - 1] = '\0';
		/*if (*input)
			add_history(input);*/
		
		tokens = ft_tokenize(input);
		free(input);
		input = NULL;
		if (!tokens)
			break ;
		print_tokens(tokens);
		envp_exec = ft_build_envp_array(l_env);
		//crear un print para esta funcion y ver si se parece a envp
		
		if (!envp_exec) // check tras crear envp
		{
		        free_tokens(tokens);
        		tokens = NULL;
        		ft_printf(STDERR_FILENO, "Error: falló la creación de envp\n");
        		continue;
    		}
		process_token_expansion(&tokens, envp_exec);
		ft_free_tab(envp_exec);
		join_tokens_with_end(&tokens);
		remove_quotes_from_token_list(tokens);
		cmds = ft_parse(tokens);
		free_tokens(tokens);
		tokens = NULL;
		if (!cmds) // check tras parsear
   		 {
   		     ft_printf(STDERR_FILENO, "Error: falló el parseo\n");
   		     continue;
   		 }
		print_commands(cmds);
		// ATENCION . Valorar si las asginaciones de nuevas variables se anyaden
		// entre el parser y executer ya que en ejecucion solo hay strcut de comandos 
		/*tokens = NULL;
		if (!cmds)
			break ;
		*/
		// iterate list and execute cmds
	//	ft_execute(cmds, envp);
		//aqui ya le pasaremos la nueva struct de varaibles, no vale la pena pasar envp
		//VAR 1 = EEEE crea variable con export=0
		//export VAR1 pondra export=1
		//atencion export VAR=1 funciona
		// pero export VAR1="1" no funciona, DEBE ELIMINAR LAS COMILLAS
		
		ft_exe_pipeline(cmds, l_env);
		
		// preparacion de datos mientras no esta el parser
		
		//********************************
		//   puedes si encuentras pipe, en infile y outfile:
		//   	cmd1 (ls)
		//   		outfile: <fd pipe escritura>
		//   	cmd2 (wc -l)
		//   		infile: <fd pipe lectura>
		//
		//path = find_path(cmds->argv[0], envp);
		//ft_printf("PATH -> %s\n", path);
		//********************************
		
		//(void)envp;
		/*(void)tokens;
		cmds = NULL;
		ft_exe_tests(cmds, envp);*/
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


