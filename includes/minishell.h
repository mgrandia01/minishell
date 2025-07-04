/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:00:23 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/04 12:24:22 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include "../libft/includes/ft_printf.h"
# include "../libft/includes/get_next_line.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <errno.h>


// un nodo por comando. fin de comando si hay pipe
// para crear la lista se guardaran los tokens argv hasta encontrar un pipe. Si es un operador:
// >  guarda como outfile con append = 0
// >> outfile con append = 1
// <  infile
// << heredoc = 1 y guarda heredoc_delim
// |  termina el comando actual y comienza uno nuevo en next

typedef enum e_token_type
{
  TOKEN_WORD,
  TOKEN_PIPE,
  TOKEN_REDIR_IN,
  TOKEN_REDIR_OUT,
  TOKEN_REDIR_APPEND,
  TOKEN_HEREDOC,
  TOKEN_EOF
} t_token_type;

typedef struct s_token
{
  t_token_type  type;
  char  *value;
  int	quote; // 0 = normal, 1 = '', 2 = ""
  struct s_token  *next;
} t_token;

typedef struct s_cmd {
    char            **argv;       // Lista de argumentos: ["ls", "-l", NULL]
    int             infile;       // fd a archivo de entrada si hay redirección: "< input.txt" si hay -1 hay errores
    int             outfile;      // fd a archivo de salida: "> out.txt" o ">> out.txt"
    int             heredoc;      // 1 si heredoc, 0 si no
    char            *heredoc_delim; // delimitador del heredoc: "<< EOF"
    struct s_cmd    *next;        // siguiente comando en el pipe
}	t_cmd;

void	ft_add_history(char *input);
void	ft_setup_signals(void);
void	ft_execute(t_cmd *cmds, char *envp[]);
t_token	*ft_tokenize(char *input);
struct s_cmd	*ft_parse(t_token *tokens);
void	ft_execute(t_cmd *cmds, char *envp[]);
void	ft_free_cmds(t_cmd *cmds);

#endif
