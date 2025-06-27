/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:56:09 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/19 21:02:52 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <unistd.h>
# include <limits.h>
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <assert.h>

# include <stdbool.h>

# define S_Q '\''
# define D_Q '\"'
# define PATH_MAX 4096

typedef enum e_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}	t_gc_node;

typedef enum e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_AND,
	TOK_OR,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APPEND,
	TOK_REDIR_HEREDOC,
	TOK_OPEN_PAREN,
	TOK_CLOSE_PAREN,
	TOK_EOF
}	t_token_type;

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	char			quote_type;
}	t_token;

typedef enum e_redirect_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redirect_type;

typedef struct s_redirect
{
	t_redirect_type		type;
	char				*file;
	char				*delimiter;
	char				*heredoc_file;
	int					delimiter_quoted;
	struct s_redirect	*next;
}	t_redirect;

typedef enum e_builtin_id
{
	BI_NONE = -1,
	BI_ECHO,
	BI_CD,
	BI_PWD,
	BI_EXPORT,
	BI_UNSET,
	BI_ENV,
	BI_EXIT
}	t_builtin_id;

typedef struct s_command
{
	char			**args;
	t_builtin_id	builtin_id;
	t_redirect		*redirects;
	int				read_from_fd;
	int				write_to_fd;
	int				pipe_fd[2];
}	t_command;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
}	t_node_type;

typedef struct s_ast
{
	t_node_type		type;
	struct s_ast	*left;
	struct s_ast	*right;
	union
	{
		t_command		*cmd;
		struct s_ast	*group;
	}	u_content;
}	t_ast;

typedef struct s_shell
{
	char		**env;
	int			exit_status;
	int			running;
	char		*input_line;
	int			in_pipeline;
	t_gc_node	*gc_list;
}	t_shell;

typedef struct s_expand_ctx
{
	size_t	position_in_input;
	size_t	position_in_result;
	int		inside_single_quote;
	t_shell	*shell_instance;
	char	*expanded_line;
}	t_expand_ctx;

// ********** Execution ******************

int				set_env_var(const char *key, const char *value);
void			init_shell_level(t_shell *shell);
void			init_default_env(t_shell *shell);
int				has_redirections(t_redirect *redir);
int				handle_input_redirection(char *file);
int				handle_output_redirection(char *file, int append);
int				handle_heredoc_redirection(char *heredoc_file);
int				apply_redirections(t_redirect *redirects);
int				run_builtin(t_command *cmd, t_shell *shell);
int				backup_fds(int *in, int *out);
void			restore_fds(int in, int out);
void			reset_signals_to_default(void);
int				interpret_exit_status(int status);
int				handle_parent_process(pid_t pid, t_shell *shell, \
				t_command *cmd);
void			execute_external(t_command *cmd, t_shell *shell);
int				run_child_process(t_command *cmd, t_shell *shell);
int				run_external_or_forked_builtin(t_command *cmd, t_shell *shell);
void			print_cmd_error(const char *cmd, const char *msg, int code);
void			check_path_validity(char *path, t_command *cmd);
int				handle_builtin_command(t_command *cmd, t_shell *shell, \
				int has_redirects);
int				handle_redirections(t_command *cmd, int *in, int *out);
int				execute_command(t_command *cmd, t_shell *shell, \
				int in_pipeline);
void			child_process_left(t_ast *node, t_shell *shell, int *fds);
void			child_process_right(t_ast *node, t_shell *shell, int *fds);
int				handle_first_child(t_ast *node, t_shell *shell, int *fds);
int				handle_second_child(t_ast *node, t_shell *shell, int *fds, \
				pid_t first_pid);

void			heredoc_child(const char *delimiter, int *pipefd, int expand);
void			setup_ignore_sigint(struct sigaction *old_int);
void			restore_sigint(struct sigaction *old_int);
int				handle_heredoc_signal(int status, int *pipefd);
int				write_pipe_to_file(const char *filename, int *pipefd);

char			*ft_strstr(char *str, char *to_find);
char			*generate_heredoc_filename(void);
void			cleanup_heredoc_files(t_redirect *redir);
void			cleanup_all_heredocs(t_ast *node);
char			*ft_strndup(const char *s, size_t n);
int				is_var_char(char c);
void			append_str(char *dst, const char *src, size_t *j);
char			*get_key_value(const char *key, char **env);
int				preprocess_heredocs_in_ast(t_ast *node);
void			cleanup_all_heredocs(t_ast *node);
void			cleanup_heredoc_files(t_redirect *redir);
int				execute_ast(t_ast *node, t_shell *shell, int in_pipeline);
int				execute_shell(t_ast *ast, t_shell *shell);
char			*get_command_path(char *cmd, char **env);

// **************** GC **************

void			*gc_malloc(size_t size);
void			*gc_track(void *existing_ptr);
void			gc_free_all(void);
//shell
t_shell			*get_shell_instance(void);

//*********************Builitns ***************************/
// THE builtins..
int				builtin_echo(char **args, int fd);
int				builtin_cd(char **args, int fd);
int				builtin_pwd(char **args, int fd);
int				builtin_env(char **args, int fd);
int				builtin_export(char **args, int fd);
int				builtin_unset(char **args, int fd);
int				builtin_exit(char **args, int fd);

int				is_n_flag(char *str);
void			add_to_env(char *new_var);
void			print_var_declaration(char *var, int fd);
void			print_value(char *value, int fd);
void			print_env_in_bash_format(char **env, int fd);
int				is_valid_identifier(const char *str);
void			add_new_variable(const char *name, const char *value, \
int has_equal);
char			*get_substring_before_equal(const char *arg);
char			*get_substring_after_equal(const char *arg);
int				variable_exists(const char *name);
void			export_variable(char *arg);
int				is_append_assignment(const char *arg);
void			update_variable_value(const char *name, const char *value, \
int has_equal);
void			append_variable_value(const char *name, const char *suffix, \
int has_equal);
void			handle_existing_variable(char *name, char *value, \
				int has_equal, int is_append);
void			handle_new_variable(char *name, char *value, int has_equal);
char			**dup_env(char **env);
void			quick_sort(char *arr[], int low, int high);
int				ft_isnumeric(char *str);
int				write_with_space(int fd, char *str, int first);

char			*ft_strcpy(char *s1, char *s2);
char			*ft_strcat(char *dest, const char *src);
char			*get_env_value2(char **env, const char *key);

int				handle_too_many_args(char *arg);
void			handle_exit_argument_error(char *arg, int code);
char			*expand_variables(const char *line);

// ************************** Parsing *****************************
/* Function declarations */
t_bool			is_consecutive_operators(t_token *cur, t_token *next);
void			*gc_malloc(size_t size);
t_bool			err(char *prefix, char *token, char *suffix);
t_bool			validate_initial_tokens(t_token **tokens, size_t count);
t_bool			validate_token(t_token **tokens, size_t count);
void			gc_free_all(void);
int				validate_var_name(const char *str);
t_command		*process_command_tokens(t_token **tokens, int *i);
void			*gc_track(void *existing_ptr);
char			*until_dollar(const char *str);
char			*extract_var_name(const char *str);
int				find_var_in_env(const char *var);
char			*add_something(const char *s1, const char *s2);
char			*get_until_dollar(const char *str);
char			*ft_copy(char *str, int *i);
t_bool			set_basic_token_types(t_token *token, t_token *prv);
void			handle_command_or_option(t_command *cmd, t_token *token);
t_builtin_id	get_builtin_id(const char *arg);
void			handle_redirection_tokens(t_token *token);
void			handle_contextual_tokens(t_token *token, t_token *prv, \
				int cmd_found);
t_bool			should_expand(const char *str);
int				is_command_exists(t_token *tokens);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*ft_itoa(int n);
char			*expand_token(char *target, char **env, t_bool flag);
char			*ft_strchr(const char *s, int c);
char			*input_formating(char *input);
char			*remove_quotes(char *str);
char			**ft_split2(char *str, char *delimiters);
size_t			ft_strlen(const char *s);
t_token			*input_analyse(char *str);
void			set_tokens_type(t_token *token, t_token *prv, t_token *tokens);
t_bool			check_unclosed_quotes(char *str);
t_bool			enhanced_parse_check(t_token **tokens, size_t count, \
char *input);
t_ast			*fill_tokens(t_token **tokens);
int				ft_isalpha(int c);
char			*ft_strjoin(char const *s1, char const *s2);
char			*get_env_in_herdoc(char *target, char **env, t_bool flag,
					char *delimiter);
t_ast			*parser(char *input);
int				ft_isspace(int c);
size_t			ft_strlcpy(char *dst, const char *src, size_t size);
t_bool			is_redirection(t_token *token);
char			*ft_strtrim(char const *s1, char const *set);
t_shell			*get_shell_instance(void);
char			**dup_env(char **env);
int				ft_isalnum(int c);
int				ft_isdigit(int c);
int				ft_strcmp(const char *s1, const char *s2);
t_bool			check_unclosed_quotes(char *str);
int				check_token_sequence(t_token *current, t_token *next);
t_bool			has_redirection_error(t_token **tokens, size_t count);
char			*digit(char *target, int *i);
char			*special_cases(char *target, int *i);
char			*get_env_value(const char *name, char **env);
char			*var_expand(char *target, char **env, t_bool flag, int *i);
char			*exit_status(int *i);
void			handle_quotes_state(char c, int *in_single, int *in_double);
t_bool			is_valid_filename(const char *filename);
t_bool			iscaracter(char c);
char			*add_quotes(char *input);
char			*ft_handle_dollar_end(char *target, char *output, int *i);
int				not_expanded(char *str);
// 	*********** ss ***********************
void			reset_signals_in_child(void);
void			handle_sigint(int signum);
void			setup_signals(void);
void			setup_heredoc_signals(void);

#endif
