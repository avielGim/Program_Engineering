#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

void handle_sigint(int sig)
{
    printf("\nYou typed Control-C!\n");
}

int main()
{
    /* ignore signal ^C */
    signal(SIGINT, handle_sigint);

    /* initilize */
    char *argv[10];
    char *token, *outfile;
    char command[1024], last_command[1024];

    int i, fd, status;
    int quit = 1, amper = 0, piping = 0,
        redir_err = 0, redir_out = 0, last_status = 0;

    char prompt[1024];
    strcpy(prompt, "hello");

    /* for pipes */
    char ***args = (char ***)malloc(10 * sizeof(char **));
    int fildes[2];

    while (quit)
    {
        /* return start seeting */
        piping = 0;
        redir_out = 0;
        redir_err = 0;

        /* save the last command */
        memcpy(last_command, command, 1024);

        printf("%s: ", prompt);
        fgets(command, 1024, stdin);
        command[strlen(command) - 1] = '\0';

        /* parse command line */
        i = 0;
        token = strtok (command, " ");

        if(token != NULL && !strcmp(token, "!!"))
        {
            memcpy(command, last_command, 1024);
            token = strtok (command, " ");
        }
        
        while (token != NULL)
        {
            argv[i] = token;
            token = strtok (NULL, " ");
            i++;
            if (! piping && token && ! strcmp(token, "|"))
            {
                piping = 1;
                break;
            }
        }
        argv[i] = NULL;

        /* Is command empty */
        if (argv[0] == NULL)
            continue;

        /* exit */
        if(! strcmp(argv[0], "quit"))
        {
            quit = 0;
            continue;
        }

        /* Does command line end with & */ 
        if (! strcmp(argv[i - 1], "&"))
        {
            amper = 1;
            argv[i - 1] = NULL;
        }

        /* change directory */
        if(! strcmp(argv[0], "cd"))
        {
            if(argv[1] != NULL) chdir(argv[1]);
            else chdir("/home/aviel");
            continue;
        }

        /* get varibles enviroments */
        if (! strcmp(argv[0], "read"))
        {
            if(i > 2)
            {
                printf("too many arguments\n");
            }
            else
            {
                char read_command[1024];
                fgets(read_command, 1024, stdin);
                read_command[strlen(read_command) - 1] = '\0';

                if(i == 1)
                {
                    char temp_read[6] = "$REPLY";
                    setenv(temp_read, read_command, 1);
                }
                else if(i == 2) 
                {
                    char temp_str[1020] = "$";
                    strcat(temp_str, argv[1]);
                    
                    setenv(temp_str, read_command, 1);
                }
            }
            continue;
        }

        if(i > 1)
        {
            /* print */
            if(! strcmp(argv[0], "echo"))
            {
                /* print the last exit code */
                if(! strcmp(argv[1], "$?"))
                    printf("%d\n", last_status);

                /* also print varible's enviroment */
                else
                {
                    for (int j = 1; j < i; j++)
                    {
                        char *temp_str = argv[j];
                        if(temp_str[0] == '$' && getenv(temp_str) != NULL)
                            printf("%s ", getenv(temp_str));
                        else
                            printf("%s ", temp_str);
                    }
                    printf("\n");
                }
                continue;
            }
        }

        if(i > 2)
        {
            if(! strcmp(argv[1], "="))
            {
                /* change the prompt */
                if(! strcmp(argv[0], "prompt"))
                    strcpy(prompt, argv[2]);

                /* insert varible */
                else if(argv[0][0] == '$')
                    setenv(argv[0], argv[2], 1);

                continue;
            }

            /* redirect the stdout or stderr to file */
            redir_out = ! strcmp(argv[i - 2], ">"); // int out
            redir_err = ! strcmp(argv[i - 2], "2>"); // int err
            if (redir_out || redir_err)
            {
                argv[i - 2] = NULL;
                outfile = argv[i - 1];
            }
        }

        if (piping)
        {
            args[0] = argv;
            int command_num = 1;
            int word_num = 0;
            args[command_num] = (char **)malloc(1024 * sizeof(char *));
            token = strtok(NULL, " ");
            while (token != NULL)
            {
                if (!strcmp(token, "|"))
                {
                    args[command_num][word_num] = NULL;
                    command_num++;
                    word_num = 0;
                    args[command_num] = (char **)malloc(1024 * sizeof(char *));
                    token = strtok(NULL, " ");
                }
                args[command_num][word_num] = token;
                token = strtok(NULL, " ");
                word_num++;
            }
            args[++command_num] = NULL;
        }

        /* for commands not part of the shell command language */ 
        if (fork() == 0)
        { 
            /* redirection of stdout or stderr*/
            if (redir_err || redir_out)
            {
                fd = creat(outfile, 0660); 
                if(redir_out) close(STDOUT_FILENO);
                else close(STDERR_FILENO); 
                dup(fd); 
                close(fd);
                /* stderr or stdout | stderr is now redirected */
            }

            /* deal with many pipes */
            if (piping)
            {
                int pipe_in = 0;
                while (*args)
                {
                    pipe(fildes);
                    if (fork() == 0)
                    {
                        dup2(pipe_in, 0);
                        if (*(args + 1) != NULL)
                        {
                            dup2(fildes[1], 1);
                        }
                        close(fildes[0]);
                        execvp((*args)[0], *args);
                        exit(0);
                    }
                    else
                    {
                        wait(NULL);
                        close(fildes[1]);
                        pipe_in = fildes[0];
                        args++;
                    }
                }
                exit(0);
            }
            /* execute the command */
            else execvp(argv[0], argv);

            quit = 0;
        }

        /* parent continues here */
        
        if (amper == 0) wait(&status);

        /* save the last exit code */
        last_status = WEXITSTATUS(status);
    }
}