/*
 Bipul Karki
 */
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
// so we need to define what delimits our tokens.
// In this case  white space
// will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 5     // Mav shell only supports five arguments

#define MAX_HISTORY_SIZE 15 // maximum size of history array

#define MAX_PID_SIZE 15 // maximum size of pids array

int main()
{
    
    char* cmd_str = (char*) malloc( MAX_COMMAND_SIZE );
    char* hist[MAX_HISTORY_SIZE]; //an array for history of size 15
    int history_count = 0; // to keep track the index number of each history
    pid_t PIDs[MAX_PID_SIZE];// an array to store the pids for processes of size 15
    int pid_count=0;// pids counter
    
    while( 1 )
    {
        // Print out the msh prompt
        printf ("msh> ");
        
        // Read the command from the commandline.  The
        // maximum command that will be read is MAX_COMMAND_SIZE
        // This while command will wait here until the user
        // inputs something since fgets returns NULL when there
        // is no input
        while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );
//        if the user wants to execute the nth command from the history
//        using !n command, where this case takes !
//        as key and n as the index number to execute from the history array
        if(cmd_str[0]=='!')
        {
//            taking the index number n from !n command.
//            converting the nth command into integer and comparing 
            int index = atoi(&cmd_str[1]);
            if(index >= history_count)
            {
                perror("Command not in history: ");
            }
//            copying the command line from the history array
//            later the accessed command is executed 
            strcpy(cmd_str, hist[index]);
        }
        /* Parse input */
        char *token[MAX_NUM_ARGUMENTS];
        
        int token_count = 0;
        
        // Pointer to point to the token
        // parsed by strsep
        char *arg_ptr;
        
        char *working_str  = strdup(cmd_str);
        
        // we are going to move the working_str pointer so
        // keep track of its original value so we can deallocate
        // the correct amount at the end
        char *working_root = working_str;
        
        // Tokenize the input stringswith whitespace used as the delimiter
        while ( ( (arg_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) &&
               (token_count<MAX_NUM_ARGUMENTS))
        {
            token[token_count] = strndup( arg_ptr, MAX_COMMAND_SIZE );
            if( strlen( token[token_count] ) == 0 )
            {
                token[token_count] = NULL;
            }
            token_count++;
        }
        
        // Now print the tokenized input as a debug check
        // \TODO Remove this code and replace with your shell functionality
        
        if(strcmp(cmd_str,"\n") == 0)
        {
//            won't do anything if the input is a blank line
        }
//        exit from the shell when user provide exit/quit as input
        else if(strcmp(token[0],"exit") == 0 || strcmp(token[0],"quit")== 0)
        {
            exit(0);
        }
//       a case to perform change in directories
        else if(strcmp(token[0],"cd")==0)
        {
            if(token[1]<0)
            {
                perror(token[1]);
            }
            else
            {
                if(chdir(token[1]) == -1)
                {
                    perror("Invalid ");
                }
//                storing the command into the history array
                hist[history_count] = strdup(cmd_str);
                history_count++;
                continue; //this will help the user to goto the beginning of the loop and check all the conditions again
            }
        }
//      to print out the history of the shell commands
        else if(strcmp(token[0],"history") == 0)
        {
            hist[history_count] = strndup(cmd_str,MAX_COMMAND_SIZE);
            history_count++;
            int i;
            for(i= 0; i< history_count; i++)
            {
                printf("%d: %s\n",i,hist[i]);
            }
            continue;
        }
//        if the user wants to print the pids of the processes
        else if(strcmp(token[0],"showpids")==0)
        {
            int i;
            for(i =0; i<pid_count ; i++)
            {
                printf("%d: %d\n",i,PIDs[i]);
            }
        }
//        the fork functionality starts from here after all above conditions are false
        else
        {
            hist[history_count] = strdup(token[0]);
            history_count++;
//            forking in the new child process
            pid_t pid = fork();
            if(pid < 0)
            {
                perror("Invalid");
            }
            if(pid == 0)
            {
//               with execvp the command in token[0] is executed
                int ret = execvp(token[0], token);
//                it will return negative if the command is not valid
                if(ret == -1)
                {
                    printf("%s : Command not found.\n",token[0]);
                }
                continue;
            }
            else
            {
//                parent process
                PIDs[pid_count] = pid; // storing pid values in the PIDs array
                pid_count++;
                
                int status;
                waitpid(pid, &status, 0);
                continue;
            }
        }
        free(working_root);
    }
    return 0;
}

