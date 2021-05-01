/**
 * @author Ian Leiby
 * @author Andrew Wertz
 *
 * C file that creates the methods for internal commands.
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "internal.h"
#include "cmdline.h"
#include "error_msg.h"

/**
 * Basic internal command handler that loops through the other command names to find the right handler.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_internal(struct command *cmd, struct list_head *list){
    int i = 0;
    while(internal_cmds[i].name != 0){
        if(!strcmp(internal_cmds[i].name, cmd->cmd_name)){
            return internal_cmds[i].handler(cmd, list);
        }
        i++;
    }
    return 0;
}

/**
 * Internal command handler that handles the setenv call.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_setenv(struct command *cmd, struct list_head *list){
    if(cmd->argsNum == 1 ||cmd->argsNum > 3){
        fprintf(stderr, ERROR_SETENV_ARG);
        return -1;
    }
    setEnv(cmd->args[1], cmd->args[2], list);

    return 0;
}

/**
 * Internal command handler that handles the getenv command without any additional parameters.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_getenv(struct command *cmd, struct list_head *list){
    if(cmd->args[1] != NULL){
        handle_getenv_name(cmd, list);
    }else{
        getAllEnv(list);
    }
    
    return 0;
}

/**
 * Internal command handler that handles the getenv command with a 
 * parameter specifying what environment variable to get.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_getenv_name(struct command *cmd, struct list_head *list){
    if(cmd->argsNum > 3){
        fprintf(stderr, ERROR_GETENV_ARG);
        return -1;
    }
    char *num = getEnvValue(cmd->args[1],list);
    if(strcmp(cmd->args[1], "SUSHHOME") == 0){
        if(strcmp(num, "SUSHHOME") == 0){
            return 0;
        }else{
            return -1;
        }
    }
    if (num == NULL){
        fprintf(stderr, ERROR_GETENV_INVALID, cmd->args[1]);
        return -1;
    }
    printEnvWithValue(cmd->args[1],list);
    return 0;
}

/**
 * Internal command handler that handles the unset command.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_unsetenv_name(struct command *cmd, struct list_head *list){
    if(cmd->argsNum != 2){
        fprintf(stderr, ERROR_UNSETENV_ARG);
        return -1;
    }
    unsetEnv(cmd->args[1],list);
    return 0;
    
}

/**
 * Internal command handler that handles the cd command without any additional parameters.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_cd(struct command *cmd, struct list_head *list){
    char *path;
    
    if(cmd->args[1] != NULL){
        return handle_cd_pathname(cmd, list);
    }else{
        path = getEnvValue("HOME", list);
        if(path == NULL){
            fprintf(stderr, ERROR_CD_NOHOME);
            return -1;
        }
    }
    return chdir(path);
}

/**
 * Internal command handler that handles the cd command with parameters specifying where to cd.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_cd_pathname(struct command *cmd, struct list_head *list){
    if(cmd->argsNum != 2){
        fprintf(stderr, ERROR_CD_ARG);
        return -1;
    }

    return chdir(cmd->args[1]);
}

/**
 * Internal command handler that handles the pwd command.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_pwd(struct command *cmd, struct list_head *list){
    char buf[1024];
    if(cmd->argsNum != 1){
        fprintf(stderr, ERROR_PWD_ARG);
        return -1;
    }
    char *path = strdup(getcwd(buf, sizeof(buf)));

    if(path != NULL){
        printf("%s\n", path);
        free(path);
        return 1;
   }else {
       printf("/\n");
   }
    return 0;
}

/**
 * Internal command handler that handles the exit command.
 * @author Ian Leiby
 * @author Andrew Wertz
 * @param cmd the command that is to be executed
 * @param the linked list that represents the environment variables.
 * @return an int of whether the command was executed or not.
 **/
int handle_exit(struct command *cmd, struct list_head *list){
    if(cmd->argsNum != 1){
        fprintf(stderr, ERROR_EXIT_ARG);
        return -1;
    }
    clear_env_list(list);

     for (int c = 0; c < 1024; c++) {
           free(cmd->args[c]);
      }
    free(cmd->cmd_name);
    free(cmd->output);
    free(cmd->input);
    free(cmd->redirect_symbol);
    free(cmd);
    exit(0);
}