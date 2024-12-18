#include <pipex.h>

void execute(char *cmd, char **env)
{
    if(ft_strnstr(cmd, "' '", ft_strlen(cmd)))
    {

    }

}

int writer_child(int *pipe, char *file, char *cmd, char **env)
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
    execute(cmd, env);
}

int reader_child(int *pipe, char *file, char *cmd, char **env)
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
    execute(cmd, env);
}

int main (int ac, char **av, char **env)
{
    int pipefd[2];
    int child_read;
    int child_write;

    if( ac == 5)
    {
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        child_read = reader_child(pipefd, av[1], av[2], env);
        child_write = writer_child(pipefd, av[4], av[3], env);
        waitpid(child_read);
        waitpid(child_write);
        clean();
    }
    else
    {
        ft_printf("wrong number of arguments\n");
        exit(EXIT_FAILURE);
    }
    return(EXIT_SUCCESS);
}
