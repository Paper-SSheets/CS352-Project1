/*---------------------------------------------------------------------------
-					   COM S 352: Project 1 - Unix Shell                    -
-	Name: Steven Marshall Sheets                                            -
- 	Section: A-2                                                            -
-	NetID: smsheets@iastate.edu                                             -
-	Date: 2/5/2020                                                          -
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
-								Includes									 -
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MAX_LINE 80 /* The maximum length command */


/*
  DOCUMENTATION

  This file contains all functions in the PA1. This file parses the in put string inside of a main
  loop taking in each command line pramater. Once thoes have been parsed it creats child processes
  to exicute each command. The commands can also input and output into files aswell as pipeline
  the output of other commands into other commands.

 */

int main()
{

    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    char *args2[MAX_LINE / 2 + 1];
    args[0] = NULL;

    int should_run = 1; /* flag to determine when to exit program */

    int wordp = 0;
    int argc = 0;
    int argc2 = 0;

    int in = 0;
    int out = 0;
    int pip = 0;
    int fos = 0;

    pid_t pid = getpid();

    while (should_run)
    {
        printf("osh>");

        fflush(stdout);

        char temp[100];
        char temp2[50];

        fgets(temp, 50, stdin);

        char his[3];

        if (strlen(temp) == 3)
        {
            his[0] = temp[0];
            his[1] = temp[1];
            his[2] = 0;
        }

        if (strcmp(his, "!!") != 0)
        {
            in = 0;
            out = 0;
            pip = 0;
            fos = 0;

            if (strstr(temp, "<") != NULL)
            {
                in = 1;
            } else if (strstr(temp, ">") != NULL)
            {
                out = 1;
            } else if (strstr(temp, "|") != NULL)
            {
                pip = 1;
            }

            wordp = 0;
            argc = 0;
            argc2 = 0;

            int i = 0;

            for (i = 0; i < strlen(temp) - 1; i++)
            {
                if (i == strlen(temp) - 2)
                {
                    temp2[wordp] = temp[i];
                    wordp++;
                    temp2[wordp] = 0;
                    if (fos)
                    {
                        args2[argc2] = (char *) malloc(sizeof(wordp));
                        strcpy(args2[argc2], temp2);
                    } else
                    {
                        args[argc] = (char *) malloc(sizeof(wordp));
                        strcpy(args[argc], temp2);
                    }
                } else if (temp[i] == '<' || temp[i] == '|' || temp[i] == '>')
                {
                    wordp = 0;
                    fos = 1;
                    i++;
                    argc--;
                } else if (temp[i] == ' ')
                {
                    temp2[wordp] = 0;
                    if (fos)
                    {
                        args2[argc] = (char *) malloc(sizeof(wordp));
                        strcpy(args2[argc], temp2);
                        argc2++;
                    } else
                    {
                        args[argc] = (char *) malloc(sizeof(wordp));
                        strcpy(args[argc], temp2);
                        argc++;
                    }
                    wordp = 0;

                } else
                {
                    temp2[wordp] = temp[i];
                    wordp++;
                }

            }

            args[argc + 1] = (char *) NULL;

            args2[argc2 + 1] = (char *) NULL;

            if (!strcmp(args[0], "exit"))
            {
                should_run = 0;
            } else if (out || in)
            {

                pid_t pid = fork();

                if (pid)
                {
                    wait(NULL);
                } else
                {
                    int fd = open(args2[0], O_RDWR | O_CREAT, S_IROTH | S_IRWXU);
                    if (out)
                    {
                        dup2(fd, 1);
                    } else
                    {
                        dup2(fd, 0);
                    }
                    execvp(args[0], args);
                    printf("ERROR");
                    exit(1);
                }

            } else if (pip)
            {

                int pipefd[2];
                if (pipe(pipefd) < 0)
                {
                    printf("Pipe error!\n");
                    return 1;
                }

                int pid2 = fork();
                if (pid2 == 0)
                {
                    // Child 2
                    close(pipefd[1]);    // we don't write to the pipe read end of pipe pipfd[0]
                    dup2(pipefd[0], 0);
                    execvp(args2[0], args2);
                    printf("ERROR1");
                    exit(1);
                }

                int pid1 = fork();
                if (pid1 == 0)
                {
                    // Child 1
                    close(pipefd[0]);    // we don't read to the pipe write end of pipe pipefd[1]
                    dup2(pipefd[1], 1);
                    execvp(args[0], args);
                    printf("ERROR2");
                    exit(1);
                }

                // Parent... close pipe and wait for children
                close(pipefd[0]);  // if you take this out, we will hang!
                close(pipefd[1]);

                waitpid(pid1, NULL, 0);
                waitpid(pid2, NULL, 0);
            } else
            {

                pid_t pid = fork();

                if (pid)
                {
                    wait(NULL);
                } else
                {
                    execvp(args[0], args);
                    printf("ERROR");
                    exit(1);
                }
            }

        } else
        {
            his[0] = 0;

            if (args[0] == NULL)
            {
                printf("No commands in history\n");
            } else if (out || in)
            {

                pid_t pid = fork();

                if (pid)
                {
                    wait(NULL);
                } else
                {
                    int fd = open(args2[0], O_RDWR | O_CREAT, S_IROTH | S_IRWXU);
                    if (out)
                    {
                        dup2(fd, 1);
                    } else
                    {
                        dup2(fd, 0);
                    }
                    execvp(args[0], args);
                    printf("ERROR");
                    exit(1);
                }

            } else if (pip)
            {

                int pipefd[2];
                if (pipe(pipefd) < 0)
                {
                    printf("Pipe error!\n");
                    return 1;
                }

                int pid2 = fork();
                if (pid2 == 0)
                {
                    // Child 2
                    close(pipefd[1]);    // we don't write to the pipe read end of pipe pipfd[0]
                    dup2(pipefd[0], 0);
                    execvp(args2[0], args2);
                    printf("ERROR1");
                    exit(1);
                }

                int pid1 = fork();
                if (pid1 == 0)
                {
                    // Child 1
                    close(pipefd[0]);    // we don't read to the pipe write end of pipe pipefd[1]
                    dup2(pipefd[1], 1);
                    execvp(args[0], args);
                    printf("ERROR2");
                    exit(1);
                }

                // Parent... close pipe and wait for children
                close(pipefd[0]);  // if you take this out, we will hang!
                close(pipefd[1]);

                waitpid(pid1, NULL, 0);
                waitpid(pid2, NULL, 0);
            } else
            {

                pid_t pid = fork();

                if (pid)
                {
                    wait(NULL);
                } else
                {
                    execvp(args[0], args);
                    printf("ERROR");
                    exit(1);
                }
            }
        }
    }


    return 0;

}
