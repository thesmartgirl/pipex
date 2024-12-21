#include "pipex.h"

// void execute(char *cmd)
// {
//     char *command;

//     char *args[] = {"ls", "-l", "-a", (char *)0};
//     char *env_args[] = {"PATH=/bin", (char*)0};

//     execve("ls", args, env_args);
// }

void cmd2(int pipe[], char *file, char *cmd)
{
    int outfile;

    if (close(pipe[1]) == -1)
      exit(EXIT_FAILURE);
    if (dup2(pipe[0], STDIN_FILENO) == -1)
      exit(EXIT_FAILURE);
    close(pipe[0]);
    outfile = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outfile != -1)
    {
        if( dup2(outfile, STDOUT_FILENO) == -1)
            exit(EXIT_FAILURE);
        close(outfile);
    }
    char *args[] = {"grep", "h", NULL};
    // char *env_args[] = {"PATH=/bin", (char*)0};
    execve(ft_strjoin("/bin/", cmd), args, NULL) ;
}

void cmd1(int pipe[], char *file, char *cmd)
{
    int infile;

    if (close(pipe[0]) == -1)
        exit(EXIT_FAILURE);
    if (dup2(pipe[1], STDOUT_FILENO) == -1)
        exit(EXIT_FAILURE);
    close(pipe[1]);
    infile = open(file, O_RDONLY);
    if (infile != -1)
    {
        if( dup2(infile, STDIN_FILENO) == -1)
            exit(EXIT_FAILURE);
        close(infile);
    }
    char *args[] = {"cat", "-e", NULL};
    execve(ft_strjoin("/bin/", cmd), args, NULL);
}

int main (int ac, char **av)
{
    int pipefd[2];
    int pid;
    int status;

    if(ac != 5)
    {
        ft_printf("wrong number of arguments\n");
        return(EXIT_FAILURE);
    }
    else if (av[2][0] == '\0' | av[3][0] == '\0')
    {
        ft_printf("empty argument \n");
        return(EXIT_FAILURE);
    }

    if (pipe(pipefd) == -1)
    {
        perror("");
        return(-1);
    }

    pid = fork();
    if (pid == -1)
        exit(EXIT_FAILURE);
    if (pid == 0)
        cmd1(pipefd, av[1], av[2]);

    pid = fork();
    if (pid == -1)
        exit(EXIT_FAILURE);
    if (pid == 0)
        cmd2(pipefd, av[4], av[3]);

    close(pipefd[0]);
    close(pipefd[1]);
    wait(&status);
    wait(&status);
    // clean();

    return(WEXITSTATUS(status));
}
