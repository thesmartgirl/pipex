/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ataan <ataan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:35:09 by ataan             #+#    #+#             */
/*   Updated: 2024/12/27 19:52:56 by ataan            ###   ########.fr       */
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
	int		file_valid;
}			t_child;

#endif
