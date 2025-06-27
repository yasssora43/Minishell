
NAME = minishell
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
CFLAGS = -Wall -Wextra -Werror -Iincludes -Ilibft

SRC_FILES = minishell.c\
			ss.c\
			gc.c\
			srcs/execution/pipeline.c\
			srcs/execution/redirection_utils.c\
			srcs/execution/builtin_utils.c\
			srcs/execution/process_utils.c\
			srcs/execution/execution_flow.c\
			srcs/execution/pipeline_utils.c\
			srcs/execution/expand_heredoc.c\
			srcs/execution/expand_heredoc_utils.c\
			srcs/execution/cmd_path.c\
			srcs/execution/heredoc.c\
			srcs/execution/heredoc_utils1.c\
			srcs/execution/heredoc_utils2.c\
			srcs/execution/heredoc_utils3.c\
			srcs/builtins/env_builtins1.c\
			srcs/builtins/env_builtins2.c\
			srcs/builtins/env_builtins3.c\
			srcs/builtins/env_builtins4.c\
			srcs/builtins/builtins_utils1.c\
			srcs/builtins/builtins_utils2.c\
			srcs/builtins/builtins_utils3.c\
			srcs/builtins/builtins_utils4.c\
			srcs/builtins/builtins_utils5.c\
			srcs/builtins/builtins_utils6.c\
			srcs/builtins/builtins_utils7.c\
			srcs/builtins/builtins_utils8.c\
			srcs/utils/add_function_1.c\
			srcs/utils/add_function_2.c\
			srcs/utils/add_function_3.c\
			srcs/parsing/analiyse_2.c\
			srcs/parsing/fill.c\
			srcs/parsing/handel_qoutes.c\
			srcs/parsing/program.c\
			srcs/parsing/prs_errour.c\
			srcs/parsing/update_form.c\
			srcs/parsing/han_var.c\
			srcs/parsing/split_with_str.c\
			srcs/parsing/analyise_cammnd.c\
			srcs/parsing/fii_ast.c\
			srcs/parsing/handel_var_u.c\
			srcs/parsing/pars_cammand.c\

OBJ_FILES = $(SRC_FILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJ_FILES) $(LIBFT)
	$(CC) $(OBJ_FILES) $(LIBFT) -o $(NAME) $(CFLAGS)  -lreadline


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ_FILES)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) 
	make -C $(LIBFT_DIR) fclean


re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJ_FILES) $(LIBFT_OFILES)

