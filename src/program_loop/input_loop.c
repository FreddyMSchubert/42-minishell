/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 08:18:12 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/25 17:23:33 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int	g_sigint_received;

int	execute_input(t_program_data *program_data, char *input)
{
	t_list				*tokenified_input;
	t_bin_tree_node		*tree;
	int					valid;
	t_pid_list			*pid_list;

	if (VERBOSE == 1)
		printf("Received input: %s\n", input);
	input = expand_values(input, program_data, false);
	if (VERBOSE == 1)
	{
		printf("after expanding:\n");
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
	program_data->exit_status = valid;
	if (valid != 0)
	{
		if (VERBOSE == 1)
			printf("token sequence is invalid: %d\n", valid);
		if (!isatty(fileno(stdin)))
			program_data->exit_flag = 1;
		gc_cleanup(program_data->gc);
		program_data->gc = create_garbage_collector();
		return (-1);
	}
	if (VERBOSE == 1)
		printf("token sequence is valid\n");
	tokenified_input = switch_redir_args(tokenified_input);
	tree = tok_to_bin_tree(tokenified_input, program_data);
	tree->parent = NULL;

	if (VERBOSE == 1)
		print_binary_tree(tree, 0);
	if (VERBOSE == 1)
		printf("\n\n\n");
	// --- executer
	pid_list = NULL;
	execute(tree, program_data, &pid_list);
	wait_and_free(program_data, &pid_list);
	return (0);
}

int	run_crash_interface(t_program_data *program_data)
{
	char	*input;

	if (DEBUG == 0)
		print_logo();
	while (program_data->exit_flag == 0)
	{
		// printf("exit status: %d\n", program_data->exit_status);
		if (DEBUG == 0)
		{
			if (program_data->exit_status == 0)
			{
				printf("%s", ANSI_COLOR_CYAN);
				input = readline("crash 💣 ");
			}
			else
			{
				printf("%s", ANSI_COLOR_RED);
				input = readline("crash 💥 ");
			}
			printf("%s", ANSI_COLOR_RESET);
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
			gc_cleanup(program_data->gc);
			program_data->gc = create_garbage_collector();
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
		gc_cleanup(program_data->gc);
		program_data->gc = create_garbage_collector();
	}
	gc_cleanup(program_data->gc);
	clear_history();
	return (0);
}
