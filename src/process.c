/* process.c: PQSH Process */

#include "pqsh/macros.h"
#include "pqsh/process.h"
#include "pqsh/timestamp.h"

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

/**
 * Create new process structure given command.
 * @param   command     String with command to execute.
 * @return  Pointer to new process structure
 **/
Process *process_create(const char *command) {
    /* TODO: Implement */
    size_t size = sizeof(char) + sizeof(pid_t) + sizeof(Process*) + 3*(sizeof(double));
    Process *p = malloc(size);
    p->command = command;
    return p;
}

/**
 * Start process by forking and executing the command.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not starting the process was successful
 **/
bool process_start(Process *p) {
    /* TODO: Implement */
    p->pid = fork();
    if (p->pid < 0){
        fprintf(stderr, "Fork failed: %s\n", strerror(errno));
        return false;
    }
    else if (p->pid == 0){
        char *argv[] = {0};
        for (char *token = strtok(p->command, " "); token; token = strtok(NULL, " ")){
            argv[i++] = token;
        }
        if (!execvp(argv[0], argv)){
            return false;
        }
    }
    else{
        //Update timestamp
        p->arrival_time = timestamp();
    }

    return true;
}

/**
 * Pause process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_pause(Process *p) {
    /* TODO: Implement */
    if(kill(p->pid,SIGSTOP)){
        return true;
    }
    else{
        return false;
    }
}

/**
 * Resume process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_resume(Process *p) {
    /* TODO: Implement */
    if(kill(p->pid,SIGCONT)){
        return true;
    }
    else{
        return false;
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
