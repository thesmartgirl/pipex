#ifndef PIPEX_H
# define PIPEX_H

#include <fcntl.h>
#include "./libft/libft.h"
#include <sys/wait.h>
#include <stdio.h>

typedef struct s_child
{
  int pid;
  int status;
  bool execute_cmd;
  char **cmd;
} t_child

#endif
