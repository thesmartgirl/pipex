/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 22:50:07 by ataan             #+#    #+#             */
/*   Updated: 2025/01/27 19:33:25 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_child(t_child *child)
{
	child->last = 0;
	child->pid = -1;
	child->execute_cmd = 1;
	child->cmd = NULL;
	child->args = NULL;
}

void	manage_child1(char **av, t_child *child1, int *pipefd)
{
	int	status;

	set_cmd_args(av[2], child1);
	status = check_cmd(child1);
	if (status != EXIT_SUCCESS)
		clean_child(child1);
	if (child1->execute_cmd != 0)
	{
		child1->pid = fork();
		if (child1->pid == -1)
			clean_and_exit(EXIT_FAILURE, child1);
		if (child1->pid == 0)
			cmd1(pipefd, av[1], child1);
	}
	clean_child(child1);
}

void	manage_child2(char **av, t_child *child2, int *pipefd)
{
	int	outfile;
	int	status;

	outfile = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	set_cmd_args(av[3], child2);
	status = check_cmd(child2);
	if (status != EXIT_SUCCESS)
	{
		clean_child(child2);
		close(outfile);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(status);
	}
	if (child2->execute_cmd)
	{
		child2->pid = fork();
		if (child2->pid == -1)
			clean_and_exit(EXIT_FAILURE, child2);
		if (child2->pid == 0)
			cmd2(pipefd, outfile, child2);
	}
	clean_child(child2);
	if (outfile != -1)
		close(outfile);
}

void	cmd2(int pipe[], int outfile, t_child *child)
{
	close(pipe[1]);
	if (outfile != -1)
	{
		if (dup2(pipe[0], STDIN_FILENO) == -1)
			clean_and_exit(EXIT_FAILURE, child);
		close(pipe[0]);
		if (dup2(outfile, STDOUT_FILENO) == -1)
			clean_and_exit(EXIT_FAILURE, child);
		close(outfile);
		execve(child->cmd, child->args, NULL);
		clean_and_exit(EXIT_FAILURE, child);
	}
	else
	{
		close(pipe[0]);
		clean_and_exit(EXIT_FAILURE, child);
	}
}

void	cmd1(int pipe[], char *file, t_child *child1)
{
	int	infile;

	close(pipe[0]);
	if (dup2(pipe[1], STDOUT_FILENO) == -1)
		clean_and_exit(EXIT_FAILURE, child1);
	close(pipe[1]);
	infile = open(file, O_RDONLY);
	if (infile == -1)
		perror(file);
	else
	{
		if (dup2(infile, STDIN_FILENO) == -1)
			clean_and_exit(EXIT_FAILURE, child1);
		close(infile);
	}
	if (execve(child1->cmd, child1->args, NULL) == -1)
	{
		clean_child(child1);
		if (errno == ENOENT)
			exit(127);
		if (errno == EACCES)
			exit(126);
	}
}
