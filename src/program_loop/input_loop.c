/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 08:18:12 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/23 07:36:52 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int	g_sigint_received;

int	execute_input(t_program_data *program_data, char *input)
{
	t_list				*tokenified_input;
	t_bin_tree_node		*tree;
	int					valid;
	int					status;
	pid_t				last_pid;

	if (VERBOSE == 1)
		ft_printf("Received input: %s\n", input);
	input = expand_values(input, program_data, false);
	if (VERBOSE == 1)
	{
		ft_printf("after expanding:\n");
		printf("input: %s\n", input);
	}
	// --- lexer
	tokenified_input = lexer(input, program_data);
	if (tokenified_input == NULL)
		return (ft_putstr_fd("crash: error lexing input", STDERR_FILENO), -1);
	if (VERBOSE == 1)
		print_tokens(tokenified_input);
	// --- validator
	valid = validator(tokenified_input);
	if (valid != 0)
	{
		program_data->exit_status = valid;
		if (VERBOSE == 1)
			ft_printf("token sequence is invalid: %d\n", valid);
		cleanup_gc_and_fd(program_data);
		return (-1);
	}
	if (VERBOSE == 1)
		ft_printf("token sequence is valid\n");
	tokenified_input = switch_redir_args(tokenified_input);
	tree = tok_to_bin_tree(tokenified_input, program_data);
	tree->parent = NULL;

	if (VERBOSE == 1)
		print_binary_tree(tree, 0);
	if (VERBOSE == 1)
		ft_printf("\n\n\n");
	// --- executer
	last_pid = execute(tree, program_data);
	if (last_pid != -1)
	{
		if (VERBOSE == 1)
			ft_printf("waiting for pid %d\n", last_pid);
		waitpid(last_pid, &status, 0);
		if (program_data->exit_status == 0 && VERBOSE == 1)
			ft_printf("exit status was: %d, will be: %d\n", program_data->exit_status, WEXITSTATUS(status));
		if (program_data->exit_status == 0)
			program_data->exit_status = WEXITSTATUS(status);;
	}
	return (0);
}

int	run_crash_interface(t_program_data *program_data)
{
	char	*input;

	if (DEBUG == 0)
		print_logo();
	while (program_data->exit_flag == 0)
	{
		if (DEBUG == 0)
		{
			if (program_data->exit_status == 0)
			{
				ft_printf("%s", ANSI_COLOR_CYAN);
				input = readline("crash 💣 ");
			}
			else
			{
				ft_printf("%s", ANSI_COLOR_RED);
				input = readline("crash 💥 ");
			}
			ft_printf("%s", ANSI_COLOR_RESET);
		}
		else
		{
			if (isatty(fileno(stdin)))
				input = readline("minishell$ ");
			else
			{
				char *line;
				line = get_next_line(fileno(stdin));
				if (line == NULL)
					return (0);
				input = ft_strtrim(line, "\n");
				free(line);
			}
		}
		if (g_sigint_received == SIGINT)
		{
			g_sigint_received = 0;
			program_data->exit_status = 1;
			if (input != NULL)
				free(input);
			continue ;
		}
		if (input == NULL || ft_isspace_str_all(input) == 1)
		{
			if (input != NULL)
				gc_append_element(program_data->gc, input);
			cleanup_gc_and_fd(program_data);
			program_data->exit_status = 0;
			if (input == NULL)
				break ;
			continue ;
		}
		else
		{
			gc_append_element(program_data->gc, input);
			add_history(input);
		}
		execute_input(program_data, input);
		cleanup_gc_and_fd(program_data);
	}
	gc_cleanup(program_data->gc);
	fd_collector_cleanup(program_data->fd_gc);
	rl_clear_history();
	return (0);
}
