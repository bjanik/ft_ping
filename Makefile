# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bjanik <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/07/27 13:47:13 by bjanik            #+#    #+#              #
#    Updated: 2020/07/27 13:47:15 by bjanik           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ping

CC = gcc

FLAGS = -Wall -Wextra -Werror

LIBFT_DIR  = libft/

LIBFT_LIB = $(LIBFT_DIR)libft.a
LIBFT_INC = $(LIBFT_DIR)includes/


SRC_PATH = srcs/
OBJ_PATH = obj/
INC_PATH = includes/

SRC_NAME = checksum.c\
			main.c\
			options.c\
			recv_pong.c\
			send_ipv4.c\
			sig_alarm.c\
			sig_final.c\

OBJ_NAME = $(SRC_NAME:.c=.o)

SRC = $(addprefix $(SRC_PATH),$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH),$(OBJ_NAME))

all : $(NAME)

$(NAME) : $(OBJ)
	@make -C $(LIBFT_DIR)
	@$(CC) $(FLAGS) $(OBJ) $(LIBFT_LIB) -I $(INC_PATH) -I $(LIBFT_INC) -o $(NAME)

$(OBJ) : $(OBJ_PATH)

$(OBJ_PATH) :
	@mkdir -p $(OBJ_PATH)
	@mkdir -p $(dir $(OBJ))

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(FLAGS) -c $< -o $@ -I$(INC_PATH) -I$(LIBFT_INC)

clean :
	/bin/rm -rf $(OBJ_PATH)
	make clean -C $(LIBFT_DIR)

fclean : clean
	make fclean -C $(LIBFT_DIR)
	/bin/rm -f $(NAME)

re : fclean all

.PHONY: all, clean, fclean, re
