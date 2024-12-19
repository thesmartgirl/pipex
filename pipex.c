#include "pipex.h"

void execute(char *cmd)
{
    if(ft_strnstr(cmd, "' '", ft_strlen(cmd)))
    {

    }

}

void cmd2(int *pipe, char *file, char *cmd)
{
    int pid;
    int outfile;
    pid = fork();
    
    if (pid == -1)
        exit(EXIT_FAILURE);
    if (close(pipe[1]) == -1)
        exit(EXIT_FAILURE);
    if (dup2(pipe[0], STDIN_FILENO) == -1)
        exit(EXIT_FAILURE);
    outfile = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outfile != -1)
    {
        if( dup2(outfile, STDOUT_FILENO) == -1)
            exit(EXIT_FAILURE);
    }
    execute(cmd);
}

void cmd1(int *pipe, char *file, char *cmd)
{
    int pid;
    int infile;

    pid = fork();
    
    if (pid == -1)
        exit(EXIT_FAILURE);
    if (close(pipe[0]) == -1)
        exit(EXIT_FAILURE);
    if (dup2(pipe[1], STDOUT_FILENO) == -1)
        exit(EXIT_FAILURE);
    infile = open(file, O_RDONLY);
    if (infile != -1)
    {
        if( dup2(infile, STDIN_FILENO) == -1)
            exit(EXIT_FAILURE);
    }
    execute(cmd);
}

int main (int ac, char **av)
{
    int pipefd[2];

    if( ac == 5)
    {
        if (pipe(pipefd) == -1)
        {
            perror("");
            return(errno);
        }
        cmd1(pipefd, av[1], av[2]);
        cmd2(pipefd, av[4], av[3]);
        wait(NULL);
        wait(NULL);
        clean();
    }
    else
    {
        ft_printf("wrong number of arguments\n");
        return(EXIT_FAILURE);
    }
    return(EXIT_SUCCESS);
}
