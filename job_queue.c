/**
* job queue attempt
* just starting basics
* not implemented or finished
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "internal.h"
#include "cmdline.h"
#include "error_msg.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * enqueues a command to be added to the queue(a linked list)
 * @param cmd is the command to be added
 * @param Q_list is the linked list to be added to
 * @param env is the environment array to be passed to execution
 *
 **/
void enqueue(struct command *cmd, struct list_head *Q_list, char **env)
{

    struct command *comm = calloc(6,sizeof(struct command));
    strcpy(comm->cmd_name,cmd->args[0]);
    for (int i = 0; i<argsNum; i++)
    {
        strcpy(comm->args[i], cmd->args[i]); 
    }
 
    list_add(comm->list, Q_list);
    
}

/**
 * runs the command in the first slot of the queue
 * @param Q_list is the linked list of queued commands
 * @param env is the environment array to be passed into execution
 **/

void runCommand(struct list_head *Q_list, char **env)
{
    struct list_head *curr = env->next;
    struct command *entry = list_entry(curr, struct command, list);

    if (entry->cmd_name != NULL)
    {
        launch_Queue_Fork(entry->cmd, env);
    }

}

/**
 * handles the child signal call
 * @param sig is the signal id
 **/
 //not sure how to handle it (how to scan through queue when signal is called)
void sig_handler(int sig)
    {
        if (sig == SIGCHLD)
            printf("** CHILD DIED %d**\n",sig);
    }

/**
* Runs commands when only one is present and creates fork
* @param cmd is the queued command to be run
* @param array is the environment array to be passed to execution
**/
void launch_Queue_Fork(struct command *cmd, char **array)
{
    void (*signal_handler)(int) = sig_handler;

    
    

    //signal(SIGCHLD, handler); // listens for the SIGNCHLD signal

    int status;
    pid_t p = fork();


    int in = close(STDIN_FILENO);
    //checks if closed out standard output correctly
    if (in == -1)
    {
        printf("Error closing standard input\n");
        exit(1);
    }

    int out = close(STDOUT_FILENO);
    if (out == -1) 
    {
        printf("error closing standard output\n");
        exit(1);
    }
    //opens the tmp file
    int fd = mkstemp("tempoutxxxxxx");

    //checks if pid value is < 0, to see if there is an error in creating the process
    if (p < 0)
    {
        printf("error in trying to fork the process\n");
        exit(1);
    }
    // child process because return value zero, calls exec system call
    else if (p == 0)
    {
        exe_child(cmd, array);
    }
    /** parent process because return value non-zero.
      * waits for child to finish, prints child exit code
      * exits the parent process
      **/
    else
    {
        signal(SIGCHLD, signal_handler);
        waitpid(p,&status, 0);
        //printf("Child exited: %d\n", status);
        //exit(0);
    }
}


    /**
    * performs child process execution
    * @param entry the command to be excuted
    * @param array is the environment array to be used in execution 
    **/
    void exe_child(struct command *entry, char **array)
    {


        int result = execvpe(entry->cmd_name, entry->args, array);
        if (result == -1)
        {
            printf(ERROR_EXEC_FAILED, strerror(errno));
            exit(0);
        }
    }


//void output()
//void cancel()
//void status()