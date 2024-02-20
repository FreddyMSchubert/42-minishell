/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:29:36 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/20 16:36:21 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main(int argc, char **argv, char **envp)
{
	t_program_data program_data;

	program_data.exit_flag = 0;
	// create exact copy of envp
	program_data.envcp = ft_strarrdup(envp);
	
	// test_lexer("echo'$HOME\"hello'\"asdasd\"", &program_data);
	test_expander(&program_data, lexer("\"echo $HOME\" \'echo $HOME\'", &program_data));

	(void)argc;
	(void)argv;
	(void)envp;

	return (0);
}
