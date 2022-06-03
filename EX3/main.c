#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    /* initilize */
    char command[1024];
    char *token, *argv[10], *outfile;
    int i, fd, amper = 0, redirOut = 0, retid, status, last_status = 0
        , redirErr = 0;
    
    int length_argv = 0;

    char *prompt = (char*)malloc(6);  // free. also check if returns value different from -1.
    strcpy(prompt, "hello");
    
    pid_t main_pid = getpid();

    int quit = 1;
    while (quit)
    {
        redirErr = 0; 
        redirOut = 0;
        printf("%s: ", prompt);
        fgets(command, 1024, stdin);
        command[strlen(command) - 1] = '\0';
        token = strtok(command," ");
        if(!strcmp(token, "!!")){
            printf("execute the last command: ");
            for(int j = 0; argv[j] != NULL; j++){
                printf("%s ", argv[j]);
            }
            printf("\n");
        }
        else{
            /* parse command line */  
            // token = strtok(command," ");
            i = 0;
            while (token != NULL)
            {
                if(i < length_argv) free(argv[i]);
                argv[i] = (char*)malloc(strlen(token) + 1);
                strcpy(argv[i], token);
                token = strtok (NULL, " ");
                i++;
            }

            if(i < length_argv) free(argv[i]);
            argv[i] = NULL;
            int tempLen = i;
            i++;
            while(i < length_argv){
                free(argv[i]);
                i++;
            }
            length_argv = tempLen;
        }
        // printf("5\n");
        /* Is command empty */
        if (argv[0] == NULL)
            continue;
        else if(!strcmp(argv[0], "prompt") && argv[1] != NULL && !strcmp(argv[1], "=")){
            if(argv[2] != NULL){
                free(prompt);
                int new_length = strlen(argv[2]);
                prompt = (char*)malloc(new_length + 1);  // free. also check if returns value different from -1.
                strcpy(prompt, argv[2]);
            } else {
                printf("wrong number of arguments\n");
            }
        }
        /* finish the program */
        else if(! strcmp(argv[0], "quit")){
            quit = 0;
            continue;   // ?
        }
        /* Does command line end with & */ 
        if (! strcmp(argv[length_argv - 1], "&")) {
            amper = 1;
            argv[length_argv - 1] = NULL;
        }
        // else 
        //     amper = 0; 
        if (! strcmp(argv[length_argv - 2], ">")) {
            redirOut = 1;
            argv[length_argv - 2] = NULL;
            outfile = argv[length_argv - 1];
        }
        else if (! strcmp(argv[length_argv - 2], "2>")) {               
            redirErr = 1;
            argv[length_argv - 2] = NULL;
            outfile = argv[length_argv - 1];
        }
        if(! strcmp(argv[0], "cd")){
            if(argv[1] != NULL) chdir(argv[1]);
            else chdir("/home/aviel");
            continue;
        }
        // else {
        //     redirOut = 0;
        // }
        if(! strcmp(argv[0], "echo") && ! strcmp(argv[1], "$?")){
            printf("%d\n", last_status);
            continue;
        }

        /* for commands not part of the shell command language */ 
        if (fork() == 0) { 

            /////////////////////MINE/////////////////////
            if (redirErr || redirOut) {
                printf("err = %d, out = %d\n", redirErr, redirOut);
                fd = creat(outfile, 0660); 
                redirErr? close (STDERR_FILENO) : close(STDOUT_FILENO); 
                dup(fd); 
                close(fd);
                /* stderr or stdout is now redirected */
            } 
            //////////////////////////////////////////////
            /* redirection of IO ? */
            // else if (redirOut) {
            //     fd = creat(outfile, 0660); 
            //     close (STDOUT_FILENO) ; 
            //     dup(fd); 
            //     close(fd); 
            //     /* stdout is now redirected */
            // } 
            execvp(argv[0], argv);
            quit = 0;
        }
        /* parent continues here */
        else if (amper == 0){
            retid = wait(&status);
        }
        last_status = WEXITSTATUS(status);
    }
    
    if(getpid() == main_pid){
        free(prompt);
        for(i = 0; argv[i] != NULL; i++){
            free(argv[i]);
        }
        printf("good bye\n");
    }
}




