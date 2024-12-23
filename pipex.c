#include "pipex.h"

void check_cmd(char *cmd)
{
    if (access(cmd, F_OK) == -1)
    {
        free(cmd);
        exit(127);
    }
    else if(access(cmd, X_OK) == -1)
    {
        free(cmd);
        exit(126);
    }
}

void cmd2(int pipe[], char *file, char *cmd)
{
    int outfile;
    char *cmd_path;

    close(pipe[1]);
    if (dup2(pipe[0], STDIN_FILENO) == -1)
        exit(EXIT_FAILURE);
    close(pipe[0]);
    outfile = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outfile != -1)
    {
        if (dup2(outfile, STDOUT_FILENO) == -1)
            exit(EXIT_FAILURE);
        close(outfile);
        cmd_path = ft_strjoin("/bin/", cmd);
        if (!cmd_path)
            exit(EXIT_FAILURE);
        char *args[] = {"grep", "sakjfna", NULL};
        check_cmd(cmd_path);
        execve(cmd_path, args, NULL);
    }
}

void cmd1(int pipe[], char *file, char *cmd)
{
    int infile;
    char *cmd_path;

    close(pipe[0]);
    if (dup2(pipe[1], STDOUT_FILENO) == -1)
        exit(EXIT_FAILURE);
    close(pipe[1]);
    infile = open(file, O_RDONLY);
    if (infile != -1)
    {
        if(dup2(infile, STDIN_FILENO) == -1)
            exit(EXIT_FAILURE);
        close(infile);
        cmd_path = ft_strjoin("/bin/", cmd);
        if (!cmd_path)
            exit(EXIT_FAILURE);
        char *args[] = {"cat", "-e", NULL};
        // check_cmd(cmd_path);
        execve(cmd_path, args, NULL);
    }
}

void check_args(int ac, char **av, t_child *child1, t_child *child2)
{
    child1->execute_cmd = 1;
    child1->execute_cmd = 1;
    if(ac != 5)
    {
        ft_printf("wrong number of arguments\n");
        exit(EXIT_FAILURE);
    }
    else if ((av[2][0] == '\0'))
        child1->execute_cmd = 0;
    else if ((av[3][0] == '\0'))
        child2->execute_cmd = 0;
}

int main(int ac, char **av)
{
    int pipefd[2];
    int status;
    int wait_pid;
    t_child child1;
    t_child child2;

    check_args(ac, av, &child1, &child2);
    if (pipe(pipefd) == -1)
        return(-1);
    child1.pid = fork();
    if (child1.pid == -1)
        exit(EXIT_FAILURE);
    if (child1.pid == 0)
        cmd1(pipefd, av[2], av[3]);
    child2.pid = fork();
    if (child2.pid == -1)
        exit(EXIT_FAILURE);
    if (child2.pid == 0)
        cmd2(pipefd, av[4], av[3]);
    close(pipefd[0]);
    close(pipefd[1]);
    wait_pid = wait(&status);
    if (wait_pid == child2.pid)
    {
      if (WIFEXITED(status))
        child2.status = WEXITSTATUS(status);
    }
    wait_pid = wait(&status);
    if (wait_pid == child2.pid)
    {
      if (WIFEXITED(status))
        child2.status = WEXITSTATUS(status);
    }
    return child2.status;
}
