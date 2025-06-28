# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/06 16:16:34 by mgrandia          #+#    #+#              #
#    Updated: 2025/05/24 17:32:26 by mgrandia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = cc
AR = ar rcs
RM = rm -f
FLAGS = -Wall -Wextra -Werror -g

GREEN = \033[38;5;114m   
SLATE = \033[38;5;145m 
TEAL = \033[38;5;123m 
RED = \033[0;31m
RESET = \033[0m

DIR_INC = includes/ 
DIR_OBJS = objs/
DIR_LIBFT = libft/
DIR_PRINTF = printf/
DIR_GNL = gnl/

SRC_LIBFT = ft_isalpha.c	ft_isdigit.c	ft_isalnum.c \
	    ft_isascii.c	ft_isprint.c \
      	ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c\
      	ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c\
      	ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c\
	ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c\
	ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c\
	ft_putnbr_fd.c\
	ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c\
       ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c ft_realloc.c

SRC_PRINTF = ft_printf.c printf_utils.c ft_uitoa.c decimal_print.c\
      hexa_print.c pointer_print.c text_print.c

SRC_GNL = get_next_line.c get_next_line_utils.c 

OBJS_LIBFT = $(SRC_LIBFT:%.c=$(DIR_OBJS)libft_%.o)
OBJS_PRINTF = $(SRC_PRINTF:%.c=$(DIR_OBJS)printf_%.o)
OBJS_GNL = $(SRC_GNL:%.c=$(DIR_OBJS)gnl_%.o)

DEP_FILES = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS_LIBFT) $(OBJS_PRINTF) $(OBJS_GNL)
	@$(AR) $(NAME) $(OBJS_LIBFT) $(OBJS_PRINTF) $(OBJS_GNL)	
	@echo
	@echo "$(GREEN)$(BOLD)✅ Library $(NAME) successfully created!$(RESET)"
	@echo "$(TEAL) ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)"
	@echo "$(SLATE)  ➤ Libft files compiled: $(words $(SRC_LIBFT))$(RESET)"
	@echo "$(SLATE)  ➤ Printf files compiled: $(words $(SRC_PRINTF))$(RESET)"
	@echo "$(SLATE)  ➤ Gnl files compiled: $(words $(SRC_GNL))$(RESET)"
	@echo "$(TEAL) ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)"
	@echo

$(DIR_OBJS):
	@mkdir -p $(DIR_OBJS)

$(DIR_OBJS)libft_%.o: $(DIR_LIBFT)%.c Makefile includes/libft.h | $(DIR_OBJS)
	@echo "$(SLATE) ⚙️  Compiling $(@F:libft_%.o=%.c)..."
	@$(CC) $(FLAGS) -MD -I$(DIR_INC) -c $< -o $@

$(DIR_OBJS)printf_%.o: $(DIR_PRINTF)%.c Makefile includes/ft_printf.h | $(DIR_OBJS)
	@echo "$(SLATE) ⚙️  Compiling $(@F:printf_%.o=%.c)..."
	@$(CC) $(FLAGS) -MD -I$(DIR_INC) -c $< -o $@

$(DIR_OBJS)gnl_%.o: $(DIR_GNL)%.c Makefile includes/get_next_line.h | $(DIR_OBJS)
	@echo "$(SLATE) ⚙️  Compiling $(@F:gnl_%.o=%.c)..."
	@$(CC) $(FLAGS) -MD -I$(DIR_INC) -c $< -o $@
	
clean:
	@echo "$(SLATE) 🧹 Removing object files at libft..."
	@$(RM) -r $(DIR_OBJS)
	@echo "$(SLATE) ✅ Object files removed!"

fclean: clean
	@echo "$(SLATE) 🗑️  Removing library libft.a"
	@$(RM) $(NAME)
	@echo "$(SLATE) ✅ fclean complete!"

re: fclean all
	@echo "$(SLATE) 🎉 Full recompilation complete!"

.PHONY: all clean fclean re

-include $(DEP_FILES)
