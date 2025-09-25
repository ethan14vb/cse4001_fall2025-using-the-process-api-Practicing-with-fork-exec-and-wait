# Assignment: Practicing the Process API
Practicing with fork, exec, wait. 

### Overview

In this assignment, you will practice using the Process API to create processes and run programs under Linux. The goal is to gain hands-on experience with system calls related to process management. Specifically, you will practice using the unix process API functions 'fork()', 'exec()', 'wait()', and 'exit()'. 

⚠️ Note: This is not an OS/161 assignment. You will complete it directly on Linux. 

Use the Linux in your CSE4001 container. If you are using macOS, you may use the Terminal (you may need to install development tools with C/C++ compilers). 

**Reference Reading**: Arpaci-Dusseau, *Operating Systems: Three Easy Pieces*, Chapter 5 (Process API Basics)
 👉 [Chapter 5 PDF](http://pages.cs.wisc.edu/~remzi/OSTEP/cpu-api.pdf)

---

### **Steps to Complete the Assignment**

1. **Accept the GitHub Classroom Invitation**
    [GitHub Link](https://classroom.github.com/a/FZh4BrQG)
2. **Set up your Repository**
   - Clone the assignment repository.
3. **Study the Reference Materials**
   - Read **Chapter 5**.
   - Download and explore the sample programs from the textbook repository:
      [OSTEP CPU API Code](https://github.com/remzi-arpacidusseau/ostep-code/tree/master/cpu-api).
4. **Write Your Programs**
   - Adapt the provided example code to answer the assignment questions.
   - Each program should be clear, well-commented, and compile/run correctly.
   - Add your solution source code to the repository.

5. **Prepare Your Report**
   - Answer the questions in the README.md file. You must edit the README.md file and not create another file with the answers. 
   - For each question:
     - Include your **code**.
     - Provide your **answer/explanation**.
6. **Submit Your Work via GitHub**
   - Push both your **program code** to your assignment repository.
   - This push will serve as your submission.
   - Make sure all files, answers, and screenshots are uploaded and rendered properly.








---
### Questions
1. Write a program that calls `fork()`. Before calling `fork()`, have the main process access a variable (e.g., x) and set its value to something (e.g., 100). What value is the variable in the child process? What happens to the variable when both the child and parent change the value of x?


```cpp
/* 
 * When the variable is accessed and set before the call to fork(), the value is the same across the parent and
 * children; this is because fork() will continue execution after the call but save everything that happened
 * before it. When both the parent and child change the value of X (after the fork), the value of x is unique
 * in each process.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
        int x;
        x = 100;

        int rc = fork();

        if (rc < 0)
        {
                fprintf(stderr, "Fork Failed \n");
                exit(1);
        }
        else if (rc == 0)
        {
                x = 5;
                printf("I am child %d\n", x);
        }
        else
        {
                x = 15;
                printf("I am parent %d\n", x);
        }
        return 0;
}

```


2. Write a program that opens a file (with the `open()` system call) and then calls `fork()` to create a new process. Can both the child and parent access the file descriptor returned by `open()`? What happens when they are writing to the file concurrently, i.e., at the same time?

```cpp
/* 
 * Both the parent and the child hold the same file descriptor returned by open(), but when they are writing to the file
 * concurrently the order of writes is unpredictable.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() 
{
    int fd = open("text_file.txt", O_WRONLY | O_TRUNC);
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

```

3. Write another program using `fork()`.The child process should print “hello”; the parent process should print “goodbye”. You should try to ensure that the child process always prints first; can you do this without calling `wait()` in the parent?

```cpp
/* 
 * This program works by storing the output in an intermediate text file visible to both the child and the parent processes.
 * When the child runs, it will print to the text file and the parent will see the update and then print afterwards.
 */

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


```


4. Write a program that calls `fork()` and then calls some form of `exec()` to run the program `/bin/ls`. See if you can try all of the variants of `exec()`, including (on Linux) `execl()`, `execle()`, `execlp()`, `execv()`, `execvp()`, and `execvpe()`. Why do you think there are so many variants of the same basic call?

```cpp
/* 
 * There are so many variants of the same basic call so that you have options depending on your needs and available parameters.
 */

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

```

5. Now write a program that uses `wait()` to wait for the child process to finish in the parent. What does `wait()` return? What happens if you use `wait()` in the child?

```cpp
/* 
 * wait() returns the PID of the reaped child when it succeeds, and an error code if nothing happens. When you use wait() in the child,
 * you will get an error code.
 */

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


```

6. Write a slight modification of the previous program, this time using `waitpid()` instead of `wait()`. When would `waitpid()` be useful?

```cpp
/* 
 * Nothing changes between the output of these two programs, but waitpid() would be useful for processes with multiple children.
 */

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
        pid_t child_pid = waitpid(pid, &status, 0); // reap child
        printf("Child Status %d\n", status);
    } 
    else 
    {
        // Parent
        int status;
        pid_t child_pid = waitpid(pid, &status, 0);  // reap child
        printf("Parent Status %d\n", status);
    }

    return 0;
}

```

7. Write a program that creates a child process, and then in the child closes standard output (`STDOUT FILENO`). What happens if the child calls `printf()` to print some output after closing the descriptor?

```cpp
/* 
 * When the child calls printf() to print some output, nothing appears in the output because stdout is closed. 
 */

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

```

