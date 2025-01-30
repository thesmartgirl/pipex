/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:34:50 by ataan             #+#    #+#             */
/*   Updated: 2025/01/30 20:03:20 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
	checks if cmd 1 or 2 (av[2] or av[3]) are empty
	checks if infile (av[1]) exists and readable
	checks if outfile (av[4]) is writtable (if it exists)
	updates child->execute_cmd and child->file_err accordingly
*/
void	check_args(char **av, t_child *child1, t_child *child2)
{
	if ((av[2][0] == '\0'))
		child1->execute_cmd = 0;
	if ((av[3][0] == '\0'))
	{
		child2->execute_cmd = 0;
		if(av[4][0] == '\0')
		{
			ft_printf("No such file or directory\n");
			child2->file_err = 1;
		}
	}
	if (access(av[1], R_OK) == -1)
	{
		perror(av[1]);
		child1->execute_cmd = 0;
		child1->file_err = 1;

	}
	if (access(av[4], W_OK) == -1 && access(av[4], F_OK) == 0)
	{
		perror(av[4]);
		child2->execute_cmd = 0;
		child2->file_err = 1;
	}
}

int	wait_on_child(t_child *child)
{
	int	wait_pid;
	int	status;

	if (child->status != 0)
	{
		return (child->status);
	}
	if (child->pid != -1)
	{
		wait_pid = wait(&status);
		if (wait_pid == child->pid)
		{
			if (WIFEXITED(status))
				child->status = WEXITSTATUS(status);
		}
		wait_pid = wait(&status);
		if (wait_pid == child->pid)
		{
			if (WIFEXITED(status))
				child->status = WEXITSTATUS(status);
		}
		return (child->status);
	}
	else
		return (EXIT_FAILURE);
}

int	wait_on_children(t_child *child1, t_child *child2)
{
	int	status;	
	
	if (child2->file_err == 1)
	{
		wait(&status);
		return (1);
	}
	else
	{
		if(child2->execute_cmd == 1)
		{
			wait_on_child(child2);
			return (child2->status);
		}
		else
		{
			wait_on_child(child1);
			return (child1->status);
		}
	}
	
}

int	main(int ac, char **av)
{
	int		pipefd[2];
	t_child	child1;
	t_child	child2;

	if (ac != 5)
	{
		ft_printf("Wrong number of arguments\n");
		exit(EXIT_FAILURE);
	}
	init_child(&child1);
	init_child(&child2);
	child2.last = 1;
	check_args(av, &child1, &child2);
	if (pipe(pipefd) == -1)
		return (-1);
	if (child1.execute_cmd != 0)
		manage_child1(av, &child1, pipefd);
	if (child2.execute_cmd != 0)
		manage_child2(av, &child2, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	return(wait_on_children(&child1, &child2));
}
