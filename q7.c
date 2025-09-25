#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() 
{
    pid_t pid = fork();

    if (pid < 0) 
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0) 
    {
        // Child
        close(STDOUT_FILENO);

        printf("hellooooooo\n");
    } 
    else 
    {
        // Parent
        wait(NULL);
    }

    return 0;
}
