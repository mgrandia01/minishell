/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:41:57 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/27 17:44:58 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
void	join_tokens_with_end(t_token **tokens, int here)
{
	t_token	*curr;
	t_token	*next;
	char	*joined;

	curr = *tokens;
	while (curr && curr->next)
	{
		next = curr->next;
		while (next && curr->end == 1)
		{
			if (next->type == TOKEN_EOF)
				break ;
			joined = ft_strjoin(curr->value, next->value);
			free(curr->value);
			curr->value = joined;
			if (here != 1 && next->end == 0)
				curr->end = 0;
			curr->next = next->next;
			free(next->value);
			free(next);
			next = curr->next;
		}
		curr = curr->next;
	}
}

// Initialize expansion state (quote type, index, result buffer, etc.)
void	init_exp_data(t_dat *data, char first_char, char *env[])
{
	data->quote = get_quoted_type(first_char);
	data->i = 0;
	data->result = NULL;
	data->s = 0;
	data->env = env;
}

void    p_tokens(t_token *tokens)
{
        const char *type_str;
        while (tokens)
        {
        switch (tokens->type)
        {
                case TOKEN_WORD:                type_str = "WORD"; break;
                case TOKEN_PIPE:                type_str = "PIPE"; break;
                case TOKEN_REDIR_IN:            type_str = "REDIR_IN"; break;
                case TOKEN_REDIR_OUT:           type_str = "REDIR_OUT"; break;
                case TOKEN_REDIR_APPEND:        type_str = "REDIR_APPEND"; break;
                case TOKEN_HEREDOC:             type_str = "HEREDOC"; break;
                case TOKEN_EOF:                 type_str = "EOF"; break;
                default:                        type_str = "UNKNOWN"; break;
        }
        printf("Token type: %s, value: '%s', end: %d\n", type_str, tokens->value, tokens->end);
        tokens = tokens->next;
        }
}

/// Loop through token string and process variable expansions or literals
void	p_exp(const char *t_val, t_token **n_lst, t_token *c, t_dat *d)
{
	char	*literal;
	int	end;
	
///	printf("PRE********p_exp***********\n");
//	p_tokens(c);

	while (t_val[d->i] != '\0')
	{
		end = c->end;
		if (t_val[d->i] == '$' && d->quote != 1 && t_val[d->i+1])
		{
			if (d->s > 1)
			{
				literal = literal_tok(&(d->result), &(d->s));
				add_tok(n_lst, c->type, literal, 1);
			}
			if (t_val[d->i + 1] == '?')
				handle_echo_exit_status(d);
			else
			{
				d->result = exp_var_at_index(t_val, &(d->i), d->env, c);
				if (t_val[d->i] == '$' || (t_val[d->i] == ' ' && t_val[d->i + 1] != '$'))
					end = 1;
				else 
					end = 0;
				handle_exp_result(n_lst, c, &(d->result), d->quote, end);
				d->s = 0;
			}
		}
		else
			handle_literal_ch(t_val, &(d->i), &(d->result), &(d->s));
	}
}

//FIXME
void	p_exp_all(const char *t_val, t_token **n_lst, t_token *c, t_dat *d)
{
	char	*literal;
	int	end;
	printf("entro\n");
	while (t_val[d->i] != '\0')
	{
		if (t_val[d->i] == '$' && t_val[d->i+1])
		{
			if (d->s > 0)
			{
				literal = literal_tok(&(d->result), &(d->s));
				add_tok(n_lst, c->type, literal,1);
			}
			if (t_val[d->i + 1] == '?')
				handle_echo_exit_status(d);
			else
			{
				printf("t_val = %s\n" , t_val);
				printf("end = %d\n" , c->end);
				printf("d->i = %d\n" , d->i);
				//printf("t_val[d->i]= %c, size + %d, cap enrere = %c\n", t_val[d->i], d->i, t_val[d->i+1]);
				d->result = exp_var_at_index(t_val, &(d->i), d->env, c);
				if (t_val[d->i] == '$' || (t_val[d->i] == ' ' && t_val[d->i + 1] != '$'))
					end = 1;
				else 
					end = 0;
				handle_exp_result(n_lst, c, &(d->result),d->quote,c->end);// d->quote,0);// end);
				d->s = 0;
			}
		}
		else
			handle_literal_ch(t_val, &(d->i), &(d->result), &(d->s));
	}
}


// Add final literal token if any characters were left unprocessed
void	finalize_expansion(t_token **n_lst, t_token *c, t_dat *data)
{
	
//	printf("finalize_expansion at expansion_exec.c\n");
//	p_tokens(*n_lst);
	if (data->s > 0)
	{
		add_tok(n_lst, c->type, literal_tok(&(data->result), &(data->s)), c->end);
	}
//	printf("\n");
//	p_tokens(*n_lst);
}
