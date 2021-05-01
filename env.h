//header file for environment functions and structures
#ifndef ENV_H
#define ENV_H
#include "list.h"
/**
* defines the structure of the environment linked list
* arguments: var, for the environment var and contents for its value
* list, for the creation of the linked list
**/
struct environ {
    char *var;
    char *contents;
    struct list_head list;
};

/**
 * retrieves a variable from the environment list and if it is found sends it to the function caller
 * @param var for the variable to be found
 * @param list for the list to be traversed for the variable
 * @return a char pointer for the contents of the variable
 * @author Andrew Wertz
 **/
char * getEnvValue(char *var, struct list_head *list);

/**
* reads the environment array given through the main function to a linked list
* @param en is the array given by the main function
* @param head is the linked list to be added to
* @author Andrew Wertz
**/
void readEnvironment(char **en, struct list_head *head);

/**
* sets an environment variable to the given value, or creates one if non-existant
* @param var is the variable to be set with the value
* @param value is the information to be stored in the given environment variable
* @param list is the linked list of environment variables to be searched through
* @author Andrew Wertz
**/
void setEnv(char *var, char *value, struct list_head *list);

/**
* removes a given environment var from the list
* @param var is the variable to be removed
* @param list is the list of environment variables to search through
* @author Andrew Wertz
**/
void unsetEnv(char *var, struct list_head *list);

/**
* searches through a linked list of environment vars and prints them all out
* @param list is the list of environment variables
* @author Andrew Wertz
**/
void getAllEnv(struct list_head *list);

/**
* prints out the given environment var from the list of environment vars
* @param var is the variable to be printed out
* @param list is the list of environment vars to search through
* @author Andrew Wertz, Ian Leiby
**/
void printEnvWithValue(char *var, struct list_head *list);

/**
 * clears the given linked list, removing the allocated memory from the heap
 * @param list is the pointer to the list_head structure that needs to be cleared
 * @author Andrew Wertz
 **/
void clear_env_list(struct list_head *list);

/**
* counts the environment vars in the list
* @param list is the list of nodes to be counted
* @return the number of values in the array
* @author Andrew Wertz
**/
int countEnvArgs(struct list_head *list);

/**
* makes an array of the size of the counted items in the list + 1
* from the contents of the environment list
* @param list is the list to be traversed
* @param envArray is the array to be filled by the list's contents
* @author Andrew Wertz, Morgan Williams-Burrell
**/
void makeEnvArray(struct list_head *list, char **envArray);

/**
* clears memory allocated for the array of environments
* @param array is the array to be cleared
* @param count is the number of values to be cleaned up
* @author Andrew Wertz
**/
void clear_envArray(char **array, int count);
#endif