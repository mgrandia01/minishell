/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 12:10:49 by mgrandia          #+#    #+#             */
/*   Updated: 2025/08/02 16:14:34 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>


static const char *skip_whitespace(const char *s)
{
    while (*s && (*s == ' ' || *s == '\t'))
        s++;
    return s;
}

// Función para calcular la longitud de una palabra considerando comillas
static int word_len_with_quotes(const char *s)
{
    int len = 0;
    char quote = 0;
    
    while (s[len])
    {
        if (!quote && (s[len] == '"' || s[len] == '\''))
        {
            quote = s[len];
            len++;
        }
        else if (quote && s[len] == quote)
        {
            quote = 0;
            len++;
            break; // Termina la palabra después de cerrar la comilla
        }
        else if (!quote && (s[len] == ' ' || s[len] == '\t'))
        {
            break; // Termina la palabra en espacio si no estamos en comillas
        }
        else
        {
            len++;
        }
    }
    return len;
}

// Función para contar palabras considerando comillas
static int count_words_with_quotes(const char *s)
{
    int count = 0;
    
    s = skip_whitespace(s);
    while (*s)
    {
        count++;
        s += word_len_with_quotes(s);
        s = skip_whitespace(s);
    }
    return count;
}

static char *extract_word(const char *s, int len)
{
    char *word;
    int i, j;
    
    // Si la palabra está entre comillas, las quitamos
    if (len >= 2 && ((s[0] == '"' && s[len-1] == '"') || 
                     (s[0] == '\'' && s[len-1] == '\'')))
    {
        word = malloc(len - 1); // -1 porque quitamos ambas comillas
        if (!word)
            return NULL;
        i = 1;
        j = 0;
        while (i < len - 1)
        {
            word[j] = s[i];
            i++;
            j++;
        }
        word[j] = '\0';
    }
    else
    {
        word = ft_substr(s, 0, len);
    }
    return word;
}
/*
 * Counts the number of words in a string separated by spaces or tabs.
 */
/*static int	count_words(const char *s)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && (s[i] == ' ' || s[i] == '\t'))
			i ++;
		if (s[i])
		{
			count ++;
			while (s[i] && (s[i] == ' ' || s[i] == '\t'))
				i ++;
		}
	}
	return (count);
}*/

/*
 * Returns the length of the next word until space or tab.
 */
/*static int	word_len(const char *s)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != ' ' && s[len] != '\t')
		len ++;
	return (len);
}*/

/*
 * Splits a string by whitespace (space or tab) into an array of words.
 * Returns a NULL-terminated array of strings.
 */
/*static char	**ft_split_whitespace(char const *s)
{
	char	**result;
	int		words;
	int		i;
	int		len;

	i = 0;
	if (!s)
		return (NULL);
	words = count_words(s);
	result = malloc(sizeof(char *) * (words + 1));
	if (!result)
		return (NULL);
	while (*s)
	{
		while (*s && (*s == ' ' || *s == '\t'))
			s++;
		if (*s)
		{
			len = word_len(s);
			result[i++] = ft_substr(s, 0, len);
			s += len;
		}
	}
	result[i] = NULL;
	return (result);
}*/
static char **ft_split_whitespace(char const *s)
{
    char    **result;
    int     words;
    int     i;
    int     len;
    int     debug;

    i = 0;
    if (!s)
        return (NULL);

    printf("[ft_split_whitespace] procesando: \"%s\"\n", s);

    words = count_words_with_quotes(s);
    printf("[ft_split_whitespace] palabras encontradas: %d\n", words);

    result = malloc(sizeof(char *) * (words + 1));
    if (!result)
        return (NULL);

    s = skip_whitespace(s);
    while (*s && i < words)
    {
        len = word_len_with_quotes(s);
        printf("[ft_split_whitespace] palabra %d, longitud: %d, contenido: \"", i, len);
        debug = 0;
        while (debug < len)
        {
            printf("%c", s[debug]);
            debug++;
        }
        printf("\"\n");

        result[i] = extract_word(s, len);
        printf("[ft_split_whitespace] resultado[%d]: \"%s\"\n", i, result[i]);

        i++;
        s += len;
        s = skip_whitespace(s);
    }
    result[i] = NULL;
    return (result);
}

/*
 * Frees a NULL-terminated array of strings and its elements.
 */
void	free_split_array(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++ ;
	}
	free(split);
}

// Splits a string by whitespace and adds each part as a token to the list.
// The original token type is preserved, and 'end' is set 
// to 'e_fl' for the last token.
/*void	split_tok(t_token **n_lst, t_token_type type, char *t_exp, int e_fl)
{
	char	**split;
	int		i;
	int		end;

	if (!t_exp)
		return ;
	printf("[split_tok] entrada: \"%s\"\n", t_exp);
	split = ft_split_whitespace(t_exp);
	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		printf("HOLA\n");
		end = 0;
		if (split[i + 1] == NULL)
			end = e_fl;
		add_token(n_lst, type, ft_strdup(split[i]), end);
		i++;
	}
	free_split_array(split);
}*/
void split_tok(t_token **n_lst, t_token_type type, char *t_exp, int e_fl)
{
    char    **split;
    int     i;
    int     end;
    int     debug;

    if (!t_exp)
        return ;

    printf("[split_tok] entrada: \"%s\"\n", t_exp);
    split = ft_split_whitespace(t_exp);

    if (!split)
    {
        printf("[split_tok] ERROR: ft_split_whitespace devolvió NULL\n");
        return ;
    }

    // Debug: imprimir todo el array
    printf("[split_tok] array completo:\n");
    debug = 0;
    while (split[debug])
    {
        printf("  split[%d]: \"%s\"\n", debug, split[debug]);
        debug++;
    }

    i = 0;
    while (split[i])
    {
        printf("[split_tok] procesando split[%d]: \"%s\"\n", i, split[i]);
        end = 0;
        if (split[i + 1] == NULL)
            end = e_fl;
        add_token(n_lst, type, ft_strdup(split[i]), end);
        i++;
    }
    free_split_array(split);
}
