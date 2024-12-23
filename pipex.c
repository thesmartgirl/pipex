#include "pipex.h"

void exit_err(int err)
{
    if (err == ENOENT)
        exit(127);
    else
        exit(EXIT_FAILURE);
}

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
        char *args[] = {cmd, "sakjfna", NULL};
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
        check_cmd(cmd_path);
        execve(cmd_path, args, NULL);
    }
}

void check_args(int ac, char **av)
{
    if(ac != 5)
    {
        ft_printf("wrong number of arguments\n");
        exit(EXIT_FAILURE);
    }
    else if ((av[2][0] == '\0'))
        execute_cmd1 = 0;
    else if ((av[3][0] == '\0'))
        execute_cmd2 = 0;
}

void child1(int *pid, char **av, int pipefd[2])
{
    *(pid) = fork();
    if (*(pid) == -1)
        exit(EXIT_FAILURE);
    if (*(pid) == 0)
        cmd1(pipefd, av[1], av[2]);
    else
        close(pipefd[1]);
}

int main(int ac, char **av)
{
    int pipefd[2];
    int pid1;
    int pid2;
    int status1 = 0;
    int status2 = 0;

    check_args(ac, av);
    if (pipe(pipefd) == -1)
        return(-1);
    child1(&pid1, av, pipefd);
    pid2 = fork();
    if (pid2 == -1)
        exit(EXIT_FAILURE);
    if (pid2 == 0)
        cmd2(pipefd, av[4], av[3]);
    close(pipefd[0]);
    close(pipefd[1]);
    close_std();
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status2, 0);
    if (WIFEXITED(status2))
        return(WEXITSTATUS(status2));
}
