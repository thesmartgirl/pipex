/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:34:50 by ataan             #+#    #+#             */
/*   Updated: 2024/12/25 15:48:27 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void free_array(char **arr)
{
	int i;

	i = 0;
	while(arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void clean_and_exit(int status, t_child *child)
{
	free(child->cmd);
	free_array(child->args);
	exit(status);
}

void	cmd2(int pipe[], char *file, t_child *child)
{
	int		outfile;

	close(pipe[1]);
	outfile = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile != -1)
	{
		if (dup2(pipe[0], STDIN_FILENO) == -1)
			clean_and_exit(EXIT_FAILURE, child);
		close(pipe[0]);
		if (dup2(outfile, STDOUT_FILENO) == -1)
			clean_and_exit(EXIT_FAILURE, child);
		close(outfile);
		execve(child->cmd, child->args, NULL);
	}
	else
	{
		ft_printf("%s: Permission denied\n", file);
		clean_and_exit(EXIT_FAILURE, child);
	}
}

void	cmd1(int pipe[], char *file, t_child *child)
{
	int		infile;

	close(pipe[0]);
	infile = open(file, O_RDONLY);
	if (infile != -1)
	{
		if (dup2(pipe[1], STDOUT_FILENO) == -1)
			clean_and_exit(EXIT_FAILURE, child);
		close(pipe[1]);
		if (dup2(infile, STDIN_FILENO) == -1)
			clean_and_exit(EXIT_FAILURE, child);
		close(infile);
		execve(child->cmd, child->args, NULL);
		free(child->cmd);
		free_array(child->args);
	}
	else
		perror(file);
}

/*
	splits cmd from args and checks for path and permission
	sets child->cmd, child->args, and child->execute_cmd
*/
void check_child_cmd(char *cmd, t_child *child)
{
	child->args = ft_split(cmd, ' ');
	if(!child->args)
		clean_and_exit(EXIT_FAILURE, child);
	child->cmd = ft_strjoin("/bin/", child->args[0]);
	if (!child->cmd)
		clean_and_exit(EXIT_FAILURE, child);
	if(child->execute_cmd)
	{
		if (access(child->cmd, F_OK) == -1)
		{
			ft_printf("command not found: %s\n", child->cmd);
			child->execute_cmd = 0;
			if(child->last)
				clean_and_exit(127, child);
		}
		else if (access(child->cmd, X_OK) == -1)
		{
			ft_printf("command not executable: %s\n", child->cmd);
			child->execute_cmd = 0;
			if(child->last)
				clean_and_exit(126, child);
		}
	}
}

void	check_args(int ac, char **av, t_child *child1, t_child *child2)
{
	child1->execute_cmd = 1;
	child2->execute_cmd = 1;
	if (ac != 5)
	{
		ft_printf("wrong number of arguments\n");
		exit(EXIT_FAILURE);
	}
	if ((av[2][0] == '\0'))
	{
		ft_printf("Empty command 1\n");
		child1->execute_cmd = 0;
	}
	if ((av[3][0] == '\0'))
	{
		ft_printf("Empty command 2\n");
		child2->execute_cmd = 0;
	}
	check_child_cmd(av[2], child1);
	check_child_cmd(av[3], child2);
}

int	wait_on_children(t_child *child2)
{
	int	wait_pid;
	int	status;

	if(child2->pid != -1)
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
/*
fix string cmd empty on 1
*/

int	main(int ac, char **av)
{
	int		pipefd[2];
	t_child	child1;
	t_child	child2;

	child1.last = 0;
	child2.last = 1;
	child1.pid = -1;
	child2.pid = -1;
	if (pipe(pipefd) == -1)
		return (-1);
	check_args(ac, av, &child1, &child2);
	if(child1.execute_cmd)
	{
		child1.pid = fork();
		if (child1.pid == -1)
			clean_and_exit(EXIT_FAILURE, &child1);
		if (child1.pid == 0)
		{
			cmd1(pipefd, av[1], &child1);
			clean_and_exit(0, &child1);
		}
	}
	if(child2.execute_cmd)
	{
		child2.pid = fork();
		if (child2.pid == -1)
			clean_and_exit(EXIT_FAILURE, &child2);
		if (child2.pid == 0)
			cmd2(pipefd, av[4], &child2);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	free(child1.cmd);
	free_array(child1.args);
	free(child2.cmd);
	free_array(child2.args);
	return(wait_on_children(&child2));
}
