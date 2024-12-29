/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:34:50 by ataan             #+#    #+#             */
/*   Updated: 2024/12/29 22:52:37 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	check_args(int ac, char **av, t_child *child1, t_child *child2)
{
	if (ac != 5)
	{
		ft_printf("wrong number of arguments\n");
		exit(EXIT_FAILURE);
	}
	if ((av[2][0] == '\0'))
		child1->execute_cmd = 0;
	if ((av[3][0] == '\0'))
		child2->execute_cmd = 0;
	if (access(av[1], F_OK) == -1)
		perror("");
	if (access(av[4], W_OK) == -1)
		perror(av[4]);
}

int	wait_on_children(t_child *child2)
{
	int	wait_pid;
	int	status;

	if (child2->pid != -1)
	{
		wait_pid = wait(&status);
		if (wait_pid == child2->pid)
		{
			if (WIFEXITED(status))
				child2->status = WEXITSTATUS(status);
		}
		wait_pid = wait(&status);
		if (wait_pid == child2->pid)
		{
			if (WIFEXITED(status))
				child2->status = WEXITSTATUS(status);
		}
		return (child2->status);
	}
	else
		return (127);
}

int	main(int ac, char **av)
{
	int		pipefd[2];
	t_child	child1;
	t_child	child2;

	init_child(&child1);
	init_child(&child2);
	if (pipe(pipefd) == -1)
		return (-1);
	check_args(ac, av, &child1, &child2);
	manage_child1(av, &child1, pipefd);
	manage_child2(av, &child2, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	if (child2.execute_cmd)
		return (wait_on_children(&child2));
	else
		return (EXIT_FAILURE);
}
