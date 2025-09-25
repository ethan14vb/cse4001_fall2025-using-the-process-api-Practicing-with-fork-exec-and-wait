#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FILE_NAME "q3_text.txt"
#define CHILD_MSG "hello\n"
#define PARENT_MSG "goodbye\n"

int main() 
{
    // Reset file
    FILE *file = fopen(FILE_NAME, "w");
    fclose(file);

    // Fork immediately
    pid_t pid = fork();
    if (pid < 0) 
    {
        // Error
        perror("fork");
        exit(1);
    }

    if (pid == 0) 
    {
        // If I'm the child
        FILE *file = fopen(FILE_NAME, "w");
        printf(CHILD_MSG);
        fprintf(file, CHILD_MSG);
        fclose(file);
    } 
    else 
    {
        // If I'm the parent
        char buffer[1024];

        while (1) 
        {
            // Open the file with read / append permissions
            FILE *file = fopen(FILE_NAME, "a+");

            // Search the file for the CHILD_MSG string within the buffer size
            int found = 0;
            while (fgets(buffer, sizeof(buffer), file))
            {
                if (strstr(buffer, CHILD_MSG)) 
                {
                    found = 1;
                    break;
                }
            }

            if (found) 
            {
                fprintf(file, PARENT_MSG);
                printf(PARENT_MSG);
                fclose(file);
                break;
            }

            fclose(file);

            // Wait before checking again
            sleep(1);
        }
    }
    return 0;
}
