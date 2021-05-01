/**
 * @author Ian Leiby
 * @author Andrew Wertz
 *
 * Header file that specifies the methods for internal commands.
 **/
#ifndef INTERNAL_H
#define INTERNAL_H
#include <stddef.h>
#include "cmdline.h"

/**
 * Structure that is used to handle any of the internal commands.
 * @author Ian Leiby
 **/
struct internal_cmd{
    const char *name;                                                /* Name of the internal command   */
    int (*handler)(struct command *cmd, struct list_head *head);     /* Pointer to the handler of the specific command */
};

/**
 * Basic internal command handler that loops through the other command names to find the right handler.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_internal(struct command *cmd, struct list_head *head);

/**
 * Internal command handler that handles the setenv call.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_setenv(struct command *cmd, struct list_head *head);

/**
 * Internal command handler that handles the getenv command without any additional parameters.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_getenv(struct command *cmd, struct list_head *head);

/**
 * Internal command handler that handles the getenv command with a 
 * parameter specifying what environment variable to get.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_getenv_name(struct command *cmd, struct list_head *head);

/**
 * Internal command handler that handles the unset command.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_unsetenv_name(struct command *cmd, struct list_head *head);

/**
 * Internal command handler that handles the cd command without any additional parameters.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_cd(struct command *cmd, struct list_head *head);

/**
 * Internal command handler that handles the cd command with parameters specifying where to cd.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_cd_pathname(struct command *cmd, struct list_head *head);

/**
 * Internal command handler that handles the pwd command.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_pwd(struct command *cmd, struct list_head *head);

/**
 * Internal command handler that handles the exit command.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_exit(struct command *cmd, struct list_head *head);

/**
 * List of internal command structs used for handling internal commands.
 * @author Ian Leiby
 **/
static struct internal_cmd internal_cmds[] = {
    {.name = "setenv", .handler=handle_setenv},
    {.name = "getenv", .handler=handle_getenv},
    {.name = "unsetenv", .handler=handle_unsetenv_name},
    {.name = "cd", .handler=handle_cd},
    {.name = "pwd", .handler=handle_pwd},
    {.name = "exit", .handler=handle_exit},
    0
};

#endif