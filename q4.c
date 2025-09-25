#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) 
{
    pid_t pid;

    pid = fork();
    if (pid < 0) 
    {
        // Error
        perror("fork");
        exit(1);
    }

    if (pid == 0) 
    {
        // Child
        execl("/bin/ls", "ls", "-l", (char *)NULL);

        // Error handling
        perror("execl failed");
        exit(1);
    } 
    else 
    {
        // Parent
        int status;
        waitpid(pid, &status, 0); // reap
    }

    return 0;
}
