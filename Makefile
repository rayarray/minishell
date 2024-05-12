# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/20 20:58:24 by tsankola          #+#    #+#              #
#    Updated: 2023/09/16 18:49:18 by tsankola         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
#NAME_BONUS = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
SFLAGS = -fsanitize=address -g3 -pedantic

SRCDIR = src
SRCDIR_BONUS = src_bonus

LIB_DIR = lib

OBJDIR = obj
OBJDIR_BONUS = obj

INCDIR = include
INCDIR_BONUS = include_bonus

OS := $(shell uname)

ifeq ($(OS),Linux)
  NPROCS:=$(shell grep -c ^processor /proc/cpuinfo)
endif
ifeq ($(OS),Darwin) # Assume Mac OS X
  NPROCS:=$(shell sysctl hw.ncpu | grep -o '[0-9]\+')
endif

LIB_INCDIR = $(LIB_DIR)/$(INCDIR)

#================ Readline ================
ifeq "$(OS)" "Darwin"
READLN_INC_DIR = $(HOME)/.brew/opt/readline/include
READLN_LIB_DIR = $(HOME)/.brew/opt/readline/lib
_READLN_LIB = libreadline.a
_READLN_HISTORY_LIB = libhistory.a
else ifeq "$(OS)" "Linux"
READLN_LIB_DIR = /usr/lib/x86_64-linux-gnu
READLN_INC_DIR = /usr/include/readline
_READLN_LIB = libreadline.a
_READLN_HISTORY_LIB = libhistory.a
endif
READLN_LIB = $(patsubst %, $(READLN_LIB_DIR)/%, $(_READLN_LIB))
READLN_LIB_NAME = $(patsubst lib%.a, %, $(_READLN_LIB))
READLN_HISTORY_LIB = $(patsubst %, $(READLN_LIB_DIR)/%, $(_READLN_HISTORY_LIB))
READLN_HISTORY_LIB_NAME = $(patsubst lib%.a, %, $(_READLN_HISTORY_LIB))
#================ Readline ================

_SRC =	bi_cd.c bi_echo.c bi_env.c bi_exit.c bi_export.c bi_pwd.c bi_unset.c \
		builtins.c command_evaluator.c command_executor.c command_interpreter.c \
		environ.c expander.c file_entry.c file_entry_utils.c \
		file_operations.c heredoc_parser.c input_reader.c environ2.c \
		input_reader_utilities.c minishell.c minishell_utils.c parser.c \
		parser_utils1.c parser_utils2.c pipe_utilities.c process_header.c \
		sig_handling.c temp_file.c vector.c heredoc_utils.c
SRC = $(patsubst %, $(SRCDIR)/%, $(_SRC))

_SRC_BONUS = $(patsubst %.c, %_bonus.c, $(_SRC))
SRC_BONUS = $(patsubst %, $(SRCDIR)/%, $(_SRC_BONUS))

_OBJ = $(patsubst %.c, %.o, $(_SRC))
OBJ = $(patsubst %, $(OBJDIR)/%, $(_OBJ))

_OBJ_BONUS = $(patsubst %.c, %.o, $(_SRC_BONUS))
OBJ_BONUS = $(patsubst %, $(OBJDIR)/%, $(_OBJ_BONUS))

_INC =	environ.h minishell.h sig_handling.h mini_builtins.h parser.h vector.h \
		interpreter.h file_operations.h input_reader.h heredoc.h
INC = $(patsubst %, $(INCDIR)/%, $(_INC))

_INC_BONUS = $(patsubst %.h, %_bonus.h, $(_INC))
INC_BONUS = $(patsubst %, $(INCDIR_BONUS)/%, $(_INC_BONUS))

_LIB = libft.a
LIB = $(patsubst %, $(LIB_DIR)/%, $(_LIB))
LIB_NAME = $(patsubst lib%.a, %, $(_LIB))

_LIB_INC = libft.h
LIB_INC = $(patsubst %, $(LIB_INCDIR)/%, $(_LIB_INC))

.PHONY: all bonus test clean

all: $(NAME)

#bonus: .bonus
#
#.bonus: $(OBJ_BONUS) $(LIB)
#	$(CC) $(CFLAGS) $(OBJ_BONUS) -L$(LIB_DIR) -l$(LIB_NAME) -o $(NAME_BONUS)
#	@touch .bonus
#	@rm -f .mandatory

sanitizer: $(OBJ) $(LIB) $(READLN_LIB) $(READLN_HISTORY_LIB)
	$(CC) $(CFLAGS) $(SFLAGS) $(OBJ) -L$(LIB_DIR) -l$(LIB_NAME) -L$(READLN_LIB_DIR) -l$(READLN_LIB_NAME) -l$(READLN_HISTORY_LIB_NAME) -o $(NAME)

suppress: $(SRC) $(LIB) $(INC) $(READLN_LIB) $(READLN_HISTORY_LIB)
	$(CC) $(OBJ) -L$(LIB_DIR) -l$(LIB_NAME) -L$(READLN_LIB_DIR) -l$(READLN_LIB_NAME) -l$(READLN_HISTORY_LIB_NAME) -o $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(LIB):
	$(MAKE) -j$(NPROCS) -C $(LIB_DIR)

# Object
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INC) $(LIB_INC) $(READLN_INC_DIR) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -I$(INCDIR) -I$(LIB_INCDIR) -I$(READLN_INC_DIR) -o $@

#$(OBJDIR_BONUS)/%.o: $(SRCDIR_BONUS)/%.c $(INC_BONUS) $(LIB_INC) | $(OBJDIR_BONUS)
#	$(CC) $(CFLAGS) -c $< -I$(INCDIR_BONUS) -I$(LIB_INCDIR) -o $@

# Build
$(NAME): .mandatory

.mandatory: $(OBJ) $(LIB) $(READLN_LIB) $(READLN_HISTORY_LIB)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIB_DIR) -l$(LIB_NAME) -L$(READLN_LIB_DIR) -l$(READLN_LIB_NAME) -l$(READLN_HISTORY_LIB_NAME) -o $(NAME)
	@touch .mandatory
	@rm -f .bonus

clean:
	rm -f $(OBJ)
	rm -f $(OBJ_BONUS)
	rmdir $(OBJDIR) 2> /dev/null || true
	$(MAKE) -C $(LIB_DIR) clean
	rm -f $(LIB)
	rm -f $(patsubst %, %~, $(SRC) $(SRC_BONUS))
	rm -f $(patsubst %, %~, $(INC) $(INC_BONUS))

fclean: clean
	rm -f $(NAME)
	@rm -f .bonus
	@rm -f .mandatory

re: fclean all
