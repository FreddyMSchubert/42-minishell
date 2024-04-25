/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:34:49 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/25 09:36:16 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// the paths only log if they arent null. error better not be null.
void	log_error(char *error, char *path1, char *path2)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (path1)
	{
		ft_putstr_fd(path1, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (path2)
	{
		ft_putstr_fd(path2, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (error != NULL)
		ft_putstr_fd(error, STDERR_FILENO);
	else
		ft_putstr_fd("error", STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}
