/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:34:50 by ataan             #+#    #+#             */
/*   Updated: 2024/12/26 20:00:29 by ataan            ###   ########.fr       */
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
		clean_and_exit(EXIT_FAILURE, child);
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
		clean_and_exit(EXIT_FAILURE, child);
	}
	else
	{
		perror(file);
		clean_and_exit(EXIT_FAILURE, child);
	}
}

/*
	splits cmd from args 
	sets child->cmd, child->args
*/
void set_cmd_args(char *cmd, t_child *child)
{
	child->args = ft_split(cmd, ' ');
	child->cmd = ft_strjoin("/bin/", child->args[0]);
}

/*
	checks for path and permission
	sets child->execute_cmd
*/
int check_cmd(t_child *child)
{
	if (access(child->cmd, F_OK) == -1)
	{
		ft_printf("command not found: %s\n", child->cmd);
		child->execute_cmd = 0;
		if(child->last)
			return(127);
		else
			return(EXIT_SUCCESS);
	}
	else if (access(child->cmd, X_OK) == -1)
	{
		ft_printf("command not executable: %s\n", child->cmd);
		child->execute_cmd = 0;
		if(child->last)
			return(126);
		else
			return(EXIT_SUCCESS);
	}
	child->execute_cmd = 1;
	return (EXIT_SUCCESS);
}

void	check_args(int ac, char **av, t_child *child1, t_child *child2)
{
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
	// if (set_cmd_args(av[2], child1) == EXIT_FAILURE || set_cmd_args(av[3], child2) == EXIT_FAILURE)
	// 	return (EXIT_FAILURE);
	// check_cmd(child1);
	// return (check_cmd(child2));
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

void init_child(t_child *child)
{
	child->last = 0;
	child->pid = -1;
	child->execute_cmd = 0;
}

/*
fix string cmd empty on 1
*/

int	main(int ac, char **av)
{
	int		pipefd[2];
	t_child	child1;
	t_child	child2;

	init_child(&child1);
	init_child(&child2);
	check_args(ac, av, &child1, &child2);
	set_cmd_args(av[2], &child1);
	set_cmd_args(av[3], &child2);
	if(valid_args != EXIT_SUCCESS)
	{
		free(child1.cmd);
		free_array(child1.args);
		free(child2.cmd);
		free_array(child2.args);
		exit(valid_args);
	}
	if (pipe(pipefd) == -1)
		return (-1);
	if(child1.execute_cmd)
	{
		child1.pid = fork();
		if (child1.pid == -1)
			clean_and_exit(EXIT_FAILURE, &child1);
		if (child1.pid == 0)
			cmd1(pipefd, av[1], &child1);
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
