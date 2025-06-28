/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:00:23 by mgrandia          #+#    #+#             */
/*   Updated: 2025/06/28 15:01:10 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../libft/includes/libft.h"
# include "../libft/includes/ft_printf.h"
# include "../libft/includes/get_next_line.h"

#include <readline/readline.h>
#include <readline/history.h>


typedef struct s_cmd {
    char            **argv;       // Lista de argumentos: ["ls", "-l", NULL]
    int             infile;       // fd a archivo de entrada si hay redirección: "< input.txt" si hay -1 hay errores
    int             outfile;      // fd a archivo de salida: "> out.txt" o ">> out.txt"
    int             heredoc;      // 1 si heredoc, 0 si no
    char            *heredoc_delim; // delimitador del heredoc: "<< EOF"
    struct s_cmd    *next;        // siguiente comando en el pipe
} t_cmd;

