/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:29:36 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/07 15:03:11 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main()
{
	printf("Welcome to crazy robust & advanced shell!\n");
	printf("crash 💥 <- thats how the typing prompt could look haha\n");

	test_lexer();

	return (0);
}