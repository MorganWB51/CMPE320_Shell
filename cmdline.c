/**
 * This file contain code that is responsible for break-up the users input, making/storing commands,
 * and running commands
 * @author Morgan Williams-Burrell
 * @author Ian Leiby
 * @author Andrew Wertz
 **/
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cmdline.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "error_msg.h"
#include <errno.h>
#include <fcntl.h>
#include "internal.h"


/**
 * prints each element of "todo_list"
 * @param todo_list - the list that you want to print
 * @author Morgan Williams-Burrell
 **/
void printList(struct list_head todo_list) {
    struct list_head *curr;
    struct command *entry;

    //traverses a list
    for (curr = todo_list.next; curr->next != todo_list.next; curr = curr->next)
    {
        entry = list_entry(curr, struct command, list);
        printf("the comand is %s\n", entry->cmd_name);
        printf("the input is %s\n", entry->input);
        printf("the output is %s\n", entry->output);
        printf("the redir is %s\n", entry->redirect_symbol);
        for (int i = 0; i < entry->argsNum + 1; i++)
        {
            printf("its %d args is %s\n", i + 1, entry->args[i]);
        }
    }
}


/**
 * Break "command" into space sperated words and stores them in "list"
 * @param command - the string your input was stored into
 * @param list - the linked list that the broken up string is added to
 * @author Morgan Williams-Burrell
 * @author Andrew Wertz
 * @author Ian Leiby
 **/
void breakup_command(char *command, struct list_head *list)
{
    int i = 0;
    int currentState = WHITESPACE;
    int start = 0;
    while (i < strlen(command))
    {
        if (command[i] == ' ' || command[i] == '\n')
        {
            if (currentState == QUOTES && command[i - 1] == '"') //if you are in quote state and the last index as a quote
            {
                //add work surrounded by quotes
                struct parsed_command *separatedWord = malloc(sizeof(struct parsed_command));
                separatedWord->word = strndup(command + start + 1, (i - 2 - start));
                currentState = WHITESPACE;
                list_add_tail(&separatedWord->list, list);
            }
            else if (currentState == CHAR) // if you are in quote state
            {
                //add word
                struct parsed_command *separatedWord = malloc(sizeof(struct parsed_command));
                separatedWord->word = strndup(command + start, (i - start));
                currentState = WHITESPACE;
                list_add_tail(&separatedWord->list, list);
            }
        }
        else
        {
            if (currentState == WHITESPACE)
            {
                //starting a new word
                currentState = CHAR;
                start = i;
                if (command[i] == '"')
                {
                    //if you see a " after while in whitespace state
                    currentState = QUOTES;
                }
            }
        }
        i++;
    }
}

/**
 * clears the given linked list, removing the allocated memory from the heap
 * @param list is the pointer to the list_head structure that needs to be cleared
 * @author Morgan Williams-Burrell, Andrew Wertz
 **/
void clear_parsed_list(struct list_head *list)
{
    struct parsed_command *entry;
    while (!(list_empty(list)))
    {
        entry = list_entry(list->next, struct parsed_command, list);
        list_del(&entry->list);
        free(entry->word);
        free(entry);
    }
}

/**
* counts the number of commands taken bases on the '|' symbol 
* @param list is the list of nodes to be counted
* @return the counted value
* @author Morgan Williams-Burrell
**/
int countCommands(struct list_head list)
{
    struct list_head *curr;
    struct parsed_command *entry;
    int count = 1;

    //traverses a list
    for (curr = list.next; curr->next != list.next; curr = curr->next)
    {
        entry = list_entry(curr, struct parsed_command, list);
        if (!strcmp(entry->word, "|"))
        {
            count++;
        }
    }

    return count;
}

/**
* traverses the list and stores them in the command struct
* @param list is the list of nodes to be transversed and transferred
* @param count is the number of commands to be made
* @param head the list of command that the structs are added to
* @return returns 1 if the command is good and 0 is an error occurs
* @author Morgan Williams-Burrell, Andrew Wertz
**/
int makeCommands(struct list_head list, int count, struct list_head *head)
{
    struct list_head *curr = list.next;
    struct parsed_command *entry;
    int c = 0;
    struct command *cmd;
    int redir = NONE;


    for (int i = 0; i < count; i++)
    {
        int flag = 0;
        cmd = calloc(1, sizeof(struct command)); //make a command
        int j = 0;
        while (curr->next != list.next)
        {
            entry = list_entry(curr, struct parsed_command, list);

            if (redir == OUT)
            {
                //if you are in OUT state, store the output file
                    cmd->output = strdup(entry->word);
                    redir = NONE;
            }
            else if (redir == IN)
            {
                //if you are in IN state, store the input file
                    cmd->input = strdup(entry->word);
                    redir = NONE;
            }
            else if (cmd->cmd_name == 0)
            {
                //if the command name is not empty store in the cmd_name and the first arg
                cmd->cmd_name = strdup(entry->word);
                cmd->args[j++] = strdup(entry->word);
                cmd->argsNum++;
            }
            else if (!strcmp(entry->word, "|"))
            {
                // when a pipe is detected move the list to the next position and break from the while loop 
                curr = curr->next;
                break;
            }
            else if (!strcmp(entry->word, ">"))
            {
                if (cmd->redirect_symbol == 0) {
                    // if you don't have a redirect_symbol store, store one
                    cmd->redirect_symbol = strdup(entry->word);
                    redir = OUT;
                }else {
                    // if already present though throw error and free
                     fprintf(stderr, ERROR_INVALID_CMDLINE);
                     for (int c = 0; c < 1024; c++) {
                            free(cmd->args[c]);
                            }

                        free(cmd->cmd_name);
                        free(cmd->output);
                        free(cmd->input);
                        free(cmd->redirect_symbol);
                        free(cmd);
                     return 0;
                }
            }
            else if (!strcmp(entry->word, ">>"))
            {
                // if you don't have a redirect_symbol store, store one
               if (cmd->redirect_symbol == 0) {
                    cmd->redirect_symbol = strdup(entry->word);
                    redir = OUT;
                }else {
                    // if already present though throw error and free
                     fprintf(stderr, ERROR_INVALID_CMDLINE);
                     for (int c = 0; c < 1024; c++) {
                            free(cmd->args[c]);
                        }

                        free(cmd->cmd_name);
                        free(cmd->output);
                        free(cmd->input);
                        free(cmd->redirect_symbol);
                        free(cmd);

                     return 0;
                }
            }
            else if (!strcmp(entry->word, "<"))
            {
                 // if you don't have a redirect_symbol store, store one
                if (cmd->input == 0) {
                    redir = IN;
                }else {
                    // if already present though throw error and free
                     fprintf(stderr, ERROR_INVALID_CMDLINE);
                     for (int c = 0; c < 1024; c++) {
                            free(cmd->args[c]);
                            }

                        free(cmd->cmd_name);
                        free(cmd->output);
                        free(cmd->input);
                        free(cmd->redirect_symbol);
                        free(cmd);
                     return 0;
                }
            }
            else
            {
                //add entry to the jth index of args and increase the count
                cmd->args[j++] = strdup(entry->word);
                cmd->argsNum++;
            }
            //move to the next element in the list
            curr = curr->next;
        } 
        //adds the command to the link list when exiting the while loop
        list_add_tail(&cmd->list, head);
    }
    return 1;
}
/**
* checks if command is internal command
*@param cmd is the command name to check
*@return 1 if true or 0 if false
*@author Ian Leiby
**/
int checkInternal(char *cmd)
{
    int i = 0;
    while(internal_cmds[i].name != 0){
        if(strcmp(cmd, internal_cmds[i].name) == 0){
            return 1;
        }
        i++;
    }
    return 0;
}

/**
* Runs commands when only one is present
* @param list the list of commands
* @param head the list of envirment
* @author Morgan Williams-Burrell
* @author Andrew Wertz
**/
void launchFork(struct list_head list, struct list_head *head)
{

    struct list_head *curr = list.next;
    struct command *entry = list_entry(curr, struct command, list);

    // check to see if a command is internal
    if (checkInternal(entry->cmd_name))
    {
        handle_internal(entry, head);
        return;
    }

    // makes the enviroment array
    int countE = countEnvArgs(head);
    char *enArray[countE+1];
    makeEnvArray(head, enArray);

    int status;
    pid_t p = fork();

    //checks if pid value is < 0, to see if there is an error in creating the process
    if (p < 0)
    {
        printf("error in trying to fork the process\n");
        exit(1);
    }
    // child process because return value zero, calls exec system call
    else if (p == 0)
    {
        exe_child(entry, enArray);
        clear_envArray(enArray,countE);
        clear_env_list(head);
        exit(0);
    }
    /** parent process because return value non-zero.
      * waits for child to finish, prints child exit code
      * exits the parent process
      **/
    else
    {
        waitpid(p,&status, 0);
        clear_envArray(enArray,countE);
    }
}
/**
* performs child process execution
* @param entry the command to be excuted 
* @param array the array that contains the enviroment
* @return return 0 if exec failed to execute
* @author Morgan Williams-Burrell
* @author Andrew Wertz
**/
int exe_child(struct command *entry, char **array)
{

    checkInput(entry->input);

    checkOutput(entry->output, entry->redirect_symbol);

    int result = execvpe(entry->cmd_name, entry->args, array);
    if (result == -1)
    {
        fprintf(stderr,ERROR_EXEC_FAILED, strerror(errno));
        return 0;
    }
    return 1;
}

/**
 * Check if a command has an input
 * if so, opens the input file and dups it stdin
 * @param input is the input section of the command struct
 * @author Morgan Williams-Burrell
 **/
void checkInput(char *input)
{
    if (input != 0)
    {
        //if you have an input file try and open it
        int in = open(input, O_RDONLY);
        if (in == -1)
        {
            fprintf(stderr, ERROR_EXEC_INFILE, strerror(errno));
            exit(0);
        }
        //dups to STDIN_FILENO
        dup2(in, STDIN_FILENO);
        close(in);
    }
}

/**
 * Check if a command has an output and how to handle it
 * if so, check the redirection token to see how to handle it
 * @param out is the output section of the command struct
 * @param redir is the redirection token associated with the output
 * @author Morgan Williams-Burrell
 **/
void checkOutput(char *output, char *redir)
{
    if (output != 0)
    {
        //try to close standard out
        if (close(STDOUT_FILENO) < 0)
        {
            printf("failed to close stdout\n");
        }

        if (!strcmp(">", redir))
        {
            //opens a file for the > redirection
            int out = open(output, O_RDWR | O_CREAT | O_TRUNC, 0777);
            if (out < 0)
            {
                fprintf(stderr, ERROR_EXEC_OUTFILE, strerror(errno));
                exit(0);
            }
        }
        else
        {
            //opens a file for the >> redirection
            int out = open(output, O_RDWR | O_CREAT | O_APPEND, 0777);
            if (out < 0)
            {
                fprintf(stderr, ERROR_EXEC_APPEND, strerror(errno));
                exit(0);
            }
        }
    }
}

/**
 * Runs commands when two or more a present
 * @param list the list of commands
 * @param env the list containing the enviroment
 * @author Morgan Williams-Burrell
 * @author Andrew Wertz
 **/
void launchFork2(struct list_head list, struct list_head *env)
{
    int p[2];
    pid_t pid;
    int fd_in = 0;
    struct list_head *curr = list.next;
    struct command *entry;
    // makes the array of enviromental variables
    int countE = countEnvArgs(env);
    char *enArray[countE+1];
    makeEnvArray(env, enArray);

    while (curr->next != list.next) // walk thought the list of command
    {
        entry = list_entry(curr, struct command, list);
        int status;
        pipe(p);
        if ((pid = fork()) == -1)
        {
            fprintf(stderr, "bad fork\n");
        }
        else if (pid == 0) // if you are the child
        {

            dup2(fd_in, 0); 
            if (curr != list.prev) // if you are not the last command
            {
                dup2(p[1], 1);
            }else {
                // if your last check if you are outputing to a file
                checkOutput(entry->output, entry->redirect_symbol);
                } 

            close(p[0]);

            int rc = execvpe(entry->cmd_name, entry->args, enArray);
            if (rc == -1)
            {
                fprintf(stderr, ERROR_EXEC_FAILED, strerror(errno));
                clear_envArray(enArray,countE);
                exit(0);
            }
        }
        else
        {
            waitpid(pid, &status, 0);
            close(p[1]);
            fd_in = p[0]; //save the input for the next command
            curr = curr->next;
        }
    }
    clear_envArray(enArray,countE);
}

/**
 * Frees the linked list of commands
 * @param list the list of commands
 * @author Morgan Williams-Burrell
 **/
void clear_command(struct list_head *list)
{
   struct command *entry;
   while ( !(list_empty(list))) {
      entry = list_entry(list->next, struct command, list);
      list_del(&entry->list);
      for (int c = 0; c < 1024; c++) {
           free(entry->args[c]);
      }
      free(entry->cmd_name);
      free(entry->output);
      free(entry->input);
      free(entry->redirect_symbol);
      free(entry);
    }
}