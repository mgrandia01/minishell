/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:00:23 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/11 11:33:09 by arcmarti         ###   ########.fr       */
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
	int		end; // 0 = end, 1 = no end (--include".*c"
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

//estructura para la expansion
typedef struct s_exp_data
{
    int     quote;
    int     i;
    char    *result;
    int     s;
    char    **env;
}   t_exp_data;

typedef struct s_heredoc
{
	char *delimiter;
} t_heredoc;
typedef struct s_cmd
{
	char			**argv;			// Lista de argumentos: [""ls"", "-l", NULL]
	int				infile;		// fd a archivo de entrada si hay redirección: "< input.txt" si hay -1 hay errores
//	int				err_infile;	//perror		
	int				outfile;	// fd 1:STDOUT, 2: STDERR, >=3 a archivo de salida: "> out.txt" o ">> out.txt" 
//	int				err_outfile;	//perror		
	//int				heredoc;	// 1 si heredoc, 0 si no
	//char			*heredoc_delim;		// delimitador del heredoc: "<< EOF"
	t_heredoc		*heredocs; //array dinamico
	int				heredoc_count;// cuantos hay
	
	struct s_cmd	*next;				// siguiente comando en el pipe
}	t_cmd;

typedef struct s_env
{
	char	*key;
	char	*kval;
	int		kexp;
}	t_env;

//-------------redirections.c--------------------
void	handle_operators(char *input, t_token **list, t_pos_data *data);

//---------token_list.c------------
int		add_token(t_token **lst, t_token_type type, char *val, int quote);
int		handle_quotes(char *input, t_token **list, t_pos_data *data);
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
struct s_cmd	*ft_parse(t_token *tokens);

//---------env_expansion.c-----------
char	*exp_var_at_index(const char *t_val, int *i, char *envp[]);

void	join_tokens_with_end(t_token **tokens);

//----------quotes.c-----------	
char	*remove_quotes(char *str);
void	remove_quotes_from_token_list(t_token *token);
int	process_q_cont(char *i, int *pos, char quote, t_token **tokens);

//-----------split.c-------------
void	split_tok(t_token **n_lst, t_token_type type, char *t_exp, int e_fl);

//-----------split_utils.c--------
const char	*skip_space(const char *s);
int	word_len_with_quotes(const char *s);
int	count_words_with_quotes(const char *s);
char	*extract_word(const char *s, int len);
void	free_split_array(char **split);

//-----------tokenizer.c---------
int	get_quoted_type(char c);
char	*literal_tok(char **result, int *size);
void	handle_exit_status(int *i);
void	handle_literal_char(const char *t_val, int *i, char **r, int *s);

//----------expansion_exec.c--------------------

void	init_exp_data(t_exp_data *data, char first_char, char *env[]);
void	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_exp_data *data);
void	finalize_expansion(t_token **n_lst, t_token *c, t_exp_data *data);

//-----------expansion_process.c-----------------
void	handle_exp_result(t_token **n_lst, t_token *c, char **r, int q);
void	process_token_expansion(t_token **tokens, char *envp[]);

//----------path.c------------------
char	*find_path(char *cmd, char *env[]);

//---------- signals.c----------------	
void	ft_setup_signals(void);
void	handle_sigint(int sig);

//---------- execute.c----------------
int	ft_execute_builtin(char **cmd, t_list *l_env);
int	ft_is_builtin(char *cmd);
void	ft_exe_pipeline(t_cmd *cmd, t_list *l_env);

void	ft_free_cmds(t_cmd *cmds);

extern int	g_exit_status;
int		ft_create_heredoc(const char *delim);
int		ft_strcmp(const char *s1, const char *s2);

//----------- builtin.c----------------
int		ft_builtin_pwd(void);
int		ft_builtin_cd(char **args);
int		ft_builtin_echo(char **args);
int		ft_builtin_env(char **cmd, t_list *l_env);
int		ft_builtin_exit(char **args);
int		ft_builtin_export(char **args, t_list *l_env);
int		ft_builtin_unset(char **cmd, t_list **l_env);

//----------- minishell_utils.c----------------
char	**ft_build_envp_array(t_list *l_env);
int		ft_is_numeric(const char *str);
int		ft_is_valid_key(const char *key);
int		ft_strcmp(const char *s1, const char *s2);

//----------- minishell_utils_1.c----------------
t_list	*ft_init_env(char **envp);
void	ft_free_env(void *content);
void	ft_free_tab(char **tab);
void	ft_free_argv(char **ptr);
void	ft_cmdclear(t_cmd **lst, void (*del)(char **));
void	ft_cmddelone(t_cmd *lst, void (*del)(char **));

//----------- builtin_export_2.c----------------
void	ft_export_assign_var(char *key, char *value, t_list **l_env);

//----------- heredoc.c----------------
int	ft_create_heredoc(const char *delim);

//----------- execute_2.c----------------
void	ft_handle_single_builtin(t_cmd *cmd, t_list *l_env);
void	ft_parent_process(t_cmd *cmd, int *prev_fd, int pipefd[2], int pid);
void	ft_child_process_execute(t_cmd *cmd, t_list *l_env, int pipefd[]);
void	ft_child_process(t_cmd *cmd, t_list *l_env, int pipefd[2], int prev_fd);
void	ft_process_command(t_cmd *cmd, t_list *l_env, int *prev_fd, int pipefd[]);

#endif
