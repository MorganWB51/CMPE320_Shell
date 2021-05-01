/**
 * The header file for cmdline.h
 * This file contain the states for the reading input, the command struct, the parsed_command struct
 * and the function declaration for the methods in cmdline.c 
 * @author Morgan Williams-Burrell
 */
#ifndef CMDLINE_H
#define CMDLINE_H
#include <stddef.h>
#include "list.h"
#include "env.h"
#define MAX 1024

enum state_types {
    WHITESPACE, CHAR, ENDING, QUOTES
};

enum redirection {
    OUT, IN, NONE
};

struct parsed_command {
    char *word;
    struct list_head list;
  //  enum token_types type;
};

struct command {
    char *cmd_name;
    char *args[1024];
    char *input;
    char *output;
    char *redirect_symbol;
    int argsNum;
    struct list_head list;
};

enum token_types {
  TOKEN_NORMAL, TOKEN_REDIR, TOKEN_FNAME
};



/**
 * Break "command" into space sperated words and stores them in "list"
 * @param command - the string your input was stored into
 * @param list - the linked list that the broken up string is added to
 * @author Morgan Williams-Burrell
 **/
void breakup_command(char *command, struct list_head *list);

/**
 * prints each element of "todo_list"
 * @param todo_list - the list that you want to print
 * @author Morgan Williams-Burrell
 **/
void printList(struct list_head todo_list);

/**
 * clears the given linked list, removing the allocated memory from the heap
 * @param list is the pointer to the list_head structure that needs to be cleared
 * @author Morgan Williams-Burrell
 * @author Andrew Wertz
 **/
void clear_parsed_list(struct list_head *list);

/**
 * Frees the linked list of commands
 * @param list the list of commands
 * @author Morgan Williams-Burrell
 **/
void clear_command(struct list_head *list);

/**
* counts the number of commands taken bases on the '|' symbol 
* @param list is the list of nodes to be counted
* @return the counted value
* @author Morgan Williams-Burrell
**/
int countCommands(struct list_head list);

/**
* traverses the list and stores them in the command struct
* @param list is the list of nodes to be transversed and transferred
* @param count is the number of commands to be made
* @param head the list of command that the structs are added to
* @return returns 1 if the command is good and 0 is an error occurs
* @author Morgan Williams-Burrell
**/
int makeCommands(struct list_head list, int count, struct list_head *head);

/**
* Runs commands when only one is present
* @param list the list of commands
* @param head the list of envirment
* @author Morgan Williams-Burrell
* @author Andrew Wertz
**/
void launchFork(struct list_head list, struct list_head *head);

/**
* performs child process execution
* @param entry the command to be excuted 
* @param array the array that contains the enviroment
* @return return 0 if exec failed to execute
* @author Morgan Williams-Burrell
* @author Andrew Wertz
**/
int exe_child(struct command *entry, char **array);

/**
 * Runs commands when two or more a present
 * @param list the list of commands
 * @param env the list containing the enviroment
 * @author Morgan Williams-Burrell
 * @author Andrew Wertz
 **/
void launchFork2(struct list_head list, struct list_head *env);

/**
 * Check if a command has an input
 * if so, opens the input file and dups it stdin
 * @param input is the input section of the command struct
 * @author Morgan Williams-Burrell
 **/
void checkInput(char* input);

/**
 * Check if a command has an output and how to handle it
 * if so, check the redirection token to see how to handle it
 * @param out is the output section of the command struct
 * @param redir is the redirection token associated with the output
 * @author Morgan Williams-Burrell
 **/
void checkOutput(char *output, char *redir);

#endif
