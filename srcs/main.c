/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 14:59:40 by mgrandia          #+#    #+#             */
/*   Updated: 2025/06/28 16:29:27 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// un nodo por comando. fin de comando si hay pipe
// para crear la lista se guardaran los tokens argv hasta encontrar un pipe. Si es un operador:
// >  guarda como outfile con append = 0
// >> outfile con append = 1
// <  infile
// << heredoc = 1 y guarda heredoc_delim
// |  termina el comando actual y comienza uno nuevo en next


int	main(int argc, char *argv[], )
{
    char	*input;
    char	*tokens;
    t_cmd	*cmds;

    // cambio el estado de las senyales que luego devolvere si hago forks
    ft_ setup_signals();
    while (1)
    {
        input = readline("minishell \u25B8 ");
        if (!input)
            break;
        ft_add_history(input);

        //parser and store cmds
        tokens = tokenize(input);
        free(tokens);
        
        // create list of nodes representing cmds
        cmds = parse(tokens);

        // iterate list and execute cmds
       	execute(cmds);
       	// en los forks de las ejeciones, si son hijos hay que restarurar el estado
       	// de las senales
       	//if (pid == 0)
       	//{
    	//  signal(SIGINT, SIG_DFL);
	//    signal(SIGQUIT, SIG_DFL);
	//    ...
	//}

        
        free(input);
    }
    return 0;
}
