/* pqsh.c: Process Queue Shell */

#include "pqsh/macros.h"
#include "pqsh/options.h"
#include "pqsh/scheduler.h"
#include "pqsh/signal.h"

#include <errno.h>
#include <string.h>
#include <sys/time.h>

/* Global Variables */

Scheduler PQShellScheduler = {
    .policy    = FIFO_POLICY,
    .cores     = 1,
    .timeout   = 250000,
};

/* Help Message */

void help() {
    printf("Commands:\n");
    printf("  add    command    Add command to waiting queue.\n");
    printf("  status [queue]    Display status of specified queue (default is all).\n");
    printf("  help              Display help message.\n");
    printf("  exit|quit         Exit shell.\n");
}

/* Main Execution */

int main(int argc, char *argv[]) {
    Scheduler *s = &PQShellScheduler;

    /* TODO: Parse command line options */
    if (!parse_command_line_options(argc, argv, s)){
        fprintf(stderr, "Parsing Failure");
        return EXIT_FAILURE;
    }

    /* TODO: Register signal handlers */
    if(!signal_register(SIGALRM, 0, sigalrm_handler)){
        fprintf(stderr, "Signal handlers register failed");
        return EXIT_FAILURE;
    }    

    /* TODO: Start timer interrupt */
    struct itimerval timer;
    timer.it_interval.tv_usec = s->timeout;
    timer.it_value.tv_usec = s->timeout;

    if (setitimer(ITIMER_REAL, &timer, NULL) < 0)
        return EXIT_FAILURE;

    /* TODO: Process shell comands */
    while (!feof(stdin)) {
        char command[BUFSIZ]  = "";
        char argument[BUFSIZ] = "";

        printf("\nPQSH> ");

        while (!fgets(command, BUFSIZ, stdin) && !feof(stdin));

        chomp(command);
        
        /* TODO: Handle add and status commands */
        // Call scheduler_add with sscanf()
        if (streq(command, "help")) {
            help();
        } 
        else if (streq(command, "exit") || streq(command, "quit")) {
            break;
        } 
        else if (sscanf(command, "add %[^\t\n]", argument) == 1){
            scheduler_add(s, stdout, argument);
        }
        else if (streq(command, "status")){
            scheduler_status(s, stdout, 0);
        }
        else if (sscanf(command, "status %[^\t\n]", argument) == 1){
            int flag;
            if (streq(argument, "waiting"))
                flag |= WAITING;
            else if (streq(argument, "running"))
                flag |= RUNNING;
            else if (streq(argument, "finished"))
                flag |= FINISHED;
            else
                printf("Unknown queue...\n");
            
            scheduler_status(s, stdout, flag);
        }
        else if (strlen(command)) {
            printf("Unknown command: %s\n", command);
        }
    }

    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
