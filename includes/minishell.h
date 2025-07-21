/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:00:23 by mgrandia          #+#    #+#             */
/*   Updated: 2025/07/15 15:07:05 by mgrandia         ###   ########.fr       */
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
# include <signal.h>
# include <limits.h>

// un nodo por comando. fin de comando si hay pipe
// para crear la lista se guardaran los tokens argv hasta encontrar un pipe. 
// Si es un operador:
// >  guarda como outfile con append = 0
// >> outfile con append = 1
// <  infile
// << heredoc = 1 y guarda heredoc_delim
// |  termina el comando actual y comienza uno nuevo en next

// Estructura para pasar parámetros de posición
typedef struct s_pos_data
{
	int	start;
	int	pos;
	int	state;
}	t_pos_data;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				quote; // 0 = normal, 1 = '', 2 = ""
	struct s_token	*next;
}	t_token;

typedef enum e_redir_type
{
	NONE,
	INFILE,
	OUTFILE,
	APPEND,
	HEREDOC
}	t_redir_type;

typedef struct s_cmd
{
	char			**argv;			// Lista de argumentos: [""ls"", "-l", NULL]
	int				infile;		// fd a archivo de entrada si hay redirección: "< input.txt" si hay -1 hay errores
//	int				err_infile;	//perror		
	int				outfile;	// fd 1:STDOUT, 2: STDERR, >=3 a archivo de salida: "> out.txt" o ">> out.txt" 
//	int				err_outfile;	//perror		
	int				heredoc;	// 1 si heredoc, 0 si no
	char			*heredoc_delim;		// delimitador del heredoc: "<< EOF"
	struct s_cmd	*next;				// siguiente comando en el pipe
}	t_cmd;

//-------------redirections.c--------------------

//void	handle_output_redir(char *input, t_token **list, int *pos, int state);
//void	handle_input_redir(char *input, t_token **list, int *pos, int state);
//void	handle_pipe(t_token **list, int *pos, int state);
//void	process_operator(char *input, t_token **list, t_pos_data *data);
void	handle_operators(char *input, t_token **list, t_pos_data *data);

//---------token_list.c------------

int		add_token(t_token **lst, t_token_type type, char *val, int quote);
//int		ft_get_state(char input, int state);
//int		process_quote_content(char *input, int *pos, char quote);
int		handle_quotes(char *input, t_token **list, int *pos, int *state);
void	process_previous_word(char *input, t_token **list, t_pos_data *data);

//---------white_space.c-----------

//void	skip_whitespace(char *input, t_pos_data *data);
void	handle_whitespace(char *input, t_token **list, t_pos_data *data);
void	init_tokenizer_data(t_pos_data *data, t_token **list);
void	finalize_tokenization(char *input, t_token **list, t_pos_data *data);

//------------lexer.c-------------
void	ft_add_history(char *input);
//--------------------------------
void	free_tokens(t_token *lst);
t_token	*ft_tokenize(char *input);

//-----------parse_utils.c----------
int		argv_num(char **argv);
char	**expand_argv(char **argv, int s);
t_cmd	*init_comand(void);

//----------parse.c--------------
struct s_cmd	*ft_parse(t_token *tokens, char *envp[]);



//----------path.c------------------
char	*find_path(char *cmd, char *env[]);

//---------- signals.c----------------	
void	ft_setup_signals(void);
void	handle_sigint(int sig);

//---------- execute.c----------------
void	ft_execute(t_cmd *cmds, char *envp[]);
void	ft_exe_pipeline(t_cmd *cmd, char **envp);

void	ft_free_cmds(t_cmd *cmds);

extern int	g_exit_status;
int		ft_create_heredoc(const char *delim);
int		ft_strcmp(const char *s1, const char *s2);

//----------- builtin.c----------------
int		ft_builtin_pwd(void);
int		ft_builtin_cd(char **args);
int		ft_builtin_echo(char **args);
int		ft_builtin_env(char **cmd,  char **envp);
int		ft_builtin_exit(char **args);

//----------- minishell_utils.c----------------
int	ft_is_numeric(const char *str);


#endif
