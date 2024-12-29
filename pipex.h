/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:35:09 by ataan             #+#    #+#             */
/*   Updated: 2024/12/29 22:49:05 by ataan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "./libft/libft.h"
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/wait.h>

typedef struct s_child
{
	int		pid;
	int		status;
	bool	execute_cmd;
	bool	last;
	char	*cmd;
	char	**args;
}			t_child;

void		free_array(char **arr);
void		clean_child(t_child *child);
void		clean_and_exit(int status, t_child *child);
void		set_cmd_args(char *cmd, t_child *child);
int			check_cmd(t_child *child);

void		init_child(t_child *child);
void		manage_child1(char **av, t_child *child1, int *pipefd);
void		manage_child2(char **av, t_child *child2, int *pipefd);
void		cmd2(int pipe[], int outfile, t_child *child);
void		cmd1(int pipe[], char *file, t_child *child1);

#endif
