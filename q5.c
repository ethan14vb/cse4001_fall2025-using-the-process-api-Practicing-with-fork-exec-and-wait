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
        int status;
        pid_t child_pid = wait(&status); // reap nothing
        printf("Child Status %d\n", status);
    } 
    else 
    {
        // Parent
        int status;
        pid_t child_pid = wait(&status); // reap child
        printf("Parent Status %d\n", status);
    }

    return 0;
}
