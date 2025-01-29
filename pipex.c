/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:34:50 by ataan             #+#    #+#             */
/*   Updated: 2025/01/29 19:21:33 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
	checks if cmd 1 or 2 (av[2] or av[3]) are empty
	checks if infile (av[1]) exists and readable
	checks if outfile (av[4]) is writtable (if it exists)
	updates child->execute_cmd accordingly
*/
void	check_args(int ac, char **av, t_child *child1, t_child *child2)
{
	if (ac != 5)
	{
		ft_printf("Wrong number of arguments\n");
		exit(EXIT_FAILURE);
	}
	if ((av[2][0] == '\0'))
		child1->execute_cmd = 0;
	if ((av[3][0] == '\0'))
	{
		child2->execute_cmd = 0;
		if(av[4][0] == '\0')
			ft_printf("No such file or directory\n");
	}
	if (access(av[1], R_OK) == -1)
	{
		perror(av[1]);
		child1->execute_cmd = 0;
	}
	if (access(av[4], W_OK) == -1 && access(av[4], F_OK) == 0)
	{
		perror(av[4]);
		child2->execute_cmd = 0;
		child2->status = 1;
	}
}

int	wait_on_children(t_child *child)
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

int	main(int ac, char **av)
{
	int		pipefd[2];
	t_child	child1;
	t_child	child2;

	init_child(&child1);
	init_child(&child2);
	child2.last = 1;
	check_args(ac, av, &child1, &child2);
	if (pipe(pipefd) == -1)
		return (-1);
	if (child1.execute_cmd != 0)
		manage_child1(av, &child1, pipefd);
	if (child2.execute_cmd != 0)
		manage_child2(av, &child2, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	if (child2.execute_cmd)
		return (wait_on_children(&child2));
	else if (child2.status != 0)
	{
		wait_on_children(&child1);
		return(child2.status);
	}
	else if (child1.execute_cmd)
		return (wait_on_children(&child1));
	else
		return (EXIT_FAILURE);
}
