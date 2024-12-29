/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 22:50:12 by ataan             #+#    #+#             */
/*   Updated: 2024/12/29 23:01:20 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	clean_child(t_child *child)
{
	if (child->cmd != NULL)
		free(child->cmd);
	if (child->args != NULL)
		free_array(child->args);
}

void	clean_and_exit(int status, t_child *child)
{
	clean_child(child);
	exit(status);
}

/*
	splits cmd from args
	sets child->cmd, child->args
*/
void	set_cmd_args(char *cmd, t_child *child)
{
	if (cmd[0] != '\0')
	{
		child->args = ft_split(cmd, ' ');
		child->cmd = ft_strjoin("/bin/", child->args[0]);
	}
}

/*
	checks for path and permission
	sets child->execute_cmd
*/
int	check_cmd(t_child *child)
{
	if (child->args != NULL)
	{
		if (child->args[0] == NULL)
		{
			return (127);
		}
		if (access(child->cmd, F_OK) == -1)
		{
			ft_printf("command not found: %s\n", child->cmd);
			child->execute_cmd = 0;
			if (child->last)
				return (127);
		}
		else if (access(child->cmd, X_OK) == -1)
		{
			ft_printf("command not executable: %s\n", child->cmd);
			child->execute_cmd = 0;
			if (child->last)
				return (126);
		}
		child->execute_cmd = 1;
	}
	return (EXIT_SUCCESS);
}
