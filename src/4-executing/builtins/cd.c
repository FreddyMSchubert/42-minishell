/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 06:19:23 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/02 10:09:43 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
/*
	If no arguments are given, cd changes to the user's home directory.
	Otherwise, chdir changes directory and getcwd gets the new path.
*/
int	execute_cd(t_token **tokens, t_program_data *program_data, int cmd_start_index)
{
	char	*path;
	char	*buffer;
	char	*home;
	int		ret_val;

	if (tokens[cmd_start_index + 1] == NULL)
	{
		home = get_envcp_var("HOME", program_data->envcp);
		if (home)
			path = ft_strdup(home);
		else
			return (builtin_err("cd", -3, "HOME"), 1);
	}
	else
		path = ft_strdup(tokens[cmd_start_index + 1]->value);
	ret_val = chdir(path);
	if (ret_val != 0)
		return (builtin_err("cd", -4, NULL), errno);
	buffer = getcwd(NULL, 0);
	if (!buffer)
		return (builtin_err("cd", -5, "getcwd failed"), -1);
	free(path);
	if (set_envcp_var("OLDPWD", get_envcp_var("PWD", program_data->envcp), 0, program_data) == -1)
		return (free(buffer), builtin_err("cd", -2, "OLDPWD"), -2);
	if (set_envcp_var("PWD", buffer, 0, program_data) == -1)
		return (free(buffer), builtin_err("cd", -2, "PWD"), -2);
	return (free(buffer), 0);
}
