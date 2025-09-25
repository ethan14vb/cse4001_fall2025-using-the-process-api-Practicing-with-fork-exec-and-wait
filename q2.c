#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() 
{
    int fd = open("text_file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) 
    {
        perror("open");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) 
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0) 
    {
        write(fd, "Child\n", 6);
    } 
    else 
    {
        write(fd, "Parent\n", 7);
    }

    close(fd);
    return 0;
}
