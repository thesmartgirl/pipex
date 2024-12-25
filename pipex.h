/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:35:09 by ataan             #+#    #+#             */
/*   Updated: 2024/12/25 14:28:52 by ataan            ###   ########.fr       */
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

#endif
