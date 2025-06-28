/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 08:43:16 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 18:31:24 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

char	*get_next_line(int fd);
char	*ft_buffer(int fd, char *left, int flag);
char	*ft_memcpy_gnl(char *src, size_t n);
int		ft_index_memchar(char *s, int c, size_t n);
size_t	ft_s(char *s);
char	*ft_strjoin_gnl(char *s1, char *s2);
char	*ft_strdup_gnl(char *s);

#endif
