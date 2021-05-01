/**
* environment functions for dealing with changing the environment's variables
*
**/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error_msg.h"
#include <errno.h>
#include "list.h"
#include "env.h"

/**
* reads the environment array given through the main function to a linked list
* separates the variable from its value
* @param en is the array given by the main function
* @param head is the linked list to be added to
* @author Andrew Wertz
**/
void readEnvironment(char **en, struct list_head *head)
{
    int i = 0; 
    while(en[i] != NULL) {              //runs through the env array given by main and creates room for each
        struct environ *environment = malloc(sizeof(struct environ));
        int c = 0;
        while(en[i][c] != '=')   //separates based on equal sign
            c++;
        environment->var = strndup(en[i],c);
        environment->contents = strndup(en[i] + (c+1),(strlen(en[i])-(c+1)));
        list_add(&environment->list, head);        //adds to environment list
        i++;
    }
   
}

/**
* sets an environment variable to the given value and creates a new variable if it does not exist
* @param var is the variable to be set with the value
* @param value is the information to be stored in the given environment variable
* @param env is the linked list of environment variables to be searched through
* @author Andrew Wertz
**/
void setEnv(char *varName, char *value, struct list_head *env)
{
    struct list_head *curr;
    struct environ *entry;
    int already_exists = 0;  //flag for if the variable already exists, it is set to 1
    for (curr = env->next; curr->next != env->next; curr = curr->next) {  //traverse list
        entry = list_entry(curr, struct environ, list);

        if (!strcmp(varName,entry->var))  //if variable exists then sets to new value and marks flag as 1
        {
            free(entry->contents);
            entry->contents = strdup(value);
            already_exists = 1;
        }
            
    }
    if (already_exists == 0) // this is used to create a new variable if none exists with that name
    {
        struct environ *environment = malloc(sizeof(struct environ));
        environment->var = strdup(varName);
        environment->contents = strdup(value);
        list_add_tail(&environment->list, env);
    }
    
}

/**
* removes a given environment variable from the list
* @param varName is the variable to be removed
* @param list is the list of environment variables to search through
* @author Andrew Wertz
**/
void unsetEnv(char *varName, struct list_head *list)
{
    struct list_head *curr;
    struct environ *entry;
    for (curr = list->next; curr->next != list->next; curr = curr->next) {  //traverses list
        entry = list_entry(curr, struct environ, list);

        if (!strcmp(varName, entry->var))  //compares the variable to those in the list and deletes the required variable
        {
            list_del(&entry->list);
            free(entry->var);
            free(entry->contents);
            free(entry);
        }
    }
}

/**
* searches through a linked list of environment vars and prints them all out
* @param list is the list of environment variables
* @author Andrew Wertz
**/
void getAllEnv(struct list_head *list)
{
    struct list_head *curr;
    struct environ *entry;
    for (curr = list->next; curr->next != list->next; curr = curr->next) {   //traverses the list
        entry = list_entry(curr, struct environ, list);

        printf("%s=%s\n",entry->var,entry->contents);           //prints the variable and its contents in the correct format
    }
}

/**
* prints out the given environment var from the list of environment vars
* @param var is the variable to be printed out
* @param list is the list of environment vars to search through
* @author Andrew Wertz, Ian Leiby
**/
void printEnvWithValue(char *varName, struct list_head *list)
{
    struct list_head *curr;
    struct environ *entry;
    int valid = 0;          //flag for if the variable was found or not
    for (curr = list->next; curr->next != list->next; curr = curr->next) {
        entry = list_entry(curr, struct environ, list);
        if((!strcmp(varName, entry->var)) && (!strcmp(varName, "PS1"))) {  //if the value is PS1 it sends it out using fflush
            printf("%s",entry->contents);
            fflush(stdout);
            return;
        } else 
        if (!strcmp(varName,entry->var) && (strcmp(varName, "SUSHHOME"))) {  //if the value is not sushhome and is found
            printf("%s=%s\n",entry->var, entry->contents);
            valid = 1;   //marks as a found variable
        } 
        
            
    }
    if ((valid == 0) && (strcmp(varName, "PS1")) && (strcmp(varName, "SUSHHOME")))  //if the value is not found and its not PS1 or sushhome
        fprintf(stderr,ERROR_GETENV_INVALID, varName);
     if (!strcmp(varName, "PS1")) {                              //if the value is sushhome it adds the ">" to the message sent out
         printf(">");
         fflush(stdout);
     }
}

/**
* returns the value of a certain environment variable from the list, if its found
* used for other function calls, not for implementing commands
* @param varName is the variable name to retrieve from the list
* @param list is the list of environment variables to be searched through
* @author Andrew Wertz
**/
char * getEnvValue(char *varName, struct list_head *list)
{
    struct list_head *curr;
    struct environ *entry;
    for (curr = list->next; curr->next != list->next; curr = curr->next) {   //traverses list
        entry = list_entry(curr, struct environ, list);     
        if((!strcmp(varName, entry->var)))      //compares the variable name and returns the contents
            return(entry->contents);
        }
}

 /**
 * clears the given linked list, removing the allocated memory from the heap and deleting it from the list
 * @param list is the pointer to the list_head structure that needs to be cleared
 * @author Andrew Wertz
 **/
void clear_env_list(struct list_head *list)
{
   struct environ *entry;
   //struct list_head *curr;

   while (!(list_empty(list))) {
      entry = list_entry(list->next, struct environ, list);

      list_del(&entry->list);
      free(entry->var);
      free(entry->contents);
      free(entry);

    }
}

/**
* counts the environment vars in the list
* @param list is the list of nodes to be counted
* @return the counted value
* @author Andrew Wertz
**/
int countEnvArgs(struct list_head *list)
{
    struct list_head *curr;
    struct environ *entry;
    int count = 0;
    

    //traverses a list
    for (curr = list->next; curr->next != list->next; curr = curr->next) {
        entry = list_entry(curr, struct environ, list);
        count++;
    }

    return count;
}

/**
* makes an array of the size of the counted items in the list + 1
* from the contents of the environment list
* @param list is the list to be traversed
* @param envArray is the array to be filled by the list's contents
* @author Andrew Wertz, Morgan Williams-Burrell
**/
void makeEnvArray(struct list_head *list, char **envArray)
{
    struct list_head *curr;
    struct environ *entry;
    int c = 0;
    

    //traverses a list and sets up array
    for (curr = list->next; curr->next != list->next; curr = curr->next) {      //traverses list
        entry = list_entry(curr, struct environ, list);
        envArray[c] = malloc(sizeof(char *) * 1024);        //allocates memory to array
        
        strcpy(envArray[c],entry->var);         //creates proper format for each var
        strcat(envArray[c],"=");
        
        strcat(envArray[c], entry->contents);
        c++;

    }

    envArray[c] = NULL;             //sets last value in array to NULL, as required for environment arrays
}

/**
 * clears the given environment array 
 * @param array is the char array for the environment
 * @param count is the number of values in the array that was created from heap
 * @author Andrew Wertz
 **/
void clear_envArray(char **array, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(array[i]);
    }
}
