/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 07:59:29 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/19 17:55:11 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void test_lexer(char *input)
{
	// char *input1 = "( echo \"Hello $USER\" && ( export | cat < input.txt > output.txt ) ) || ( echo $? && ls * && cd /home ) && echo \"Nested start\" && ( cd /tmp && ls ) && echo \"Nested end\"";
	// char *input = "( echo \"Hello $USER\"";

	t_token **tokens = lexer(input);

	if (tokens != NULL)
	{
		print_tokens(tokens);

		t_bin_tree_node *tree = tok_to_bin_tree(tokens);
		print_binary_tree(tree, 0);

		for (int i = 0; tokens[i] != NULL; i++)
			free(tokens[i]);
		free(tokens);
	}
	else
	{
		printf("Lexer returned NULL. Memory allocation failure or other error.\n");
	}
}
