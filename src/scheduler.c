/* scheduler.c: PQSH Scheduler */

#include "pqsh/macros.h"
#include "pqsh/scheduler.h"
#include "pqsh/timestamp.h"

#include <errno.h>
#include <sys/wait.h>

/**
 * Add new command to waiting queue.
 * @param   s	    Pointer to Scheduler structure.
 * @param   fs      File stream to write to.
 * @param   command Command string for new Process.
 **/
void scheduler_add(Scheduler *s, FILE *fs, const char *command) {
    /* TODO: Implement */
    // Double check if process create is null
    Process *p = process_create(command);
    queue_push(&s->waiting, p);
    fprintf(fs, "Added process \"%s\" to waiting queue.\n", command);
}

/**
 * Display status of queues in Scheduler.
 * @param   s	    Pointer to Scheduler structure.
 * @param   fs      File stream to write to.
 * @param   queue   Bitmask specifying which queues to display.
 **/
void scheduler_status(Scheduler *s, FILE *fs, int queue) {
    fprintf(fs, "Running = %4lu, Waiting = %4lu, Finished = %4lu, Turnaround = %05.2lf, Response = %05.2lf\n", s->running.size, s->waiting.size, s->finished.size, (s->total_turnaround_time/(double)s->finished.size), (s->total_response_time/(double)s->finished.size));
    /* TODO: Complement implementation. */
    // Add stuff to fprintf
    if ((queue & WAITING) && s->waiting.size != 0)
        queue_dump(&s->waiting, fs);
    if ((queue & RUNNING) && s->running.size != 0)
        queue_dump(&s->running, fs);
    if ((queue & FINISHED) && s->finished.size != 0)
        queue_dump(&s->finished, fs);
}

/**
 * Schedule next process using appropriate policy.
 * @param   s	    Pointer to Scheduler structure.
 **/
void scheduler_next(Scheduler *s) {
    /* TODO: Dispatch to appropriate scheduler function. */
    if (s->policy == FIFO_POLICY)
        scheduler_fifo(s);
    else if (s->policy == RDRN_POLICY)
        scheduler_rdrn(s);
}

/**
 * Wait for any children and remove from queues and update metrics.
 * @param   s	    Pointer to Scheduler structure.
 **/
void scheduler_wait(Scheduler *s) {
    /* TODO: Wait for any children without blocking:
     *
     *  - Remove process from queues.
     *  - Update Process metrics.
     *  - Update Scheduler metrics.
     **/

    // MORE NOTES
    // Metrics are s->total_turnaround_time and s->total_response_time
    pid_t pid;
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0){
        Process *found = queue_remove(&s->running, pid);
        // If process is NULL, then check waiting
        if (found == NULL){
            found = queue_remove(&s->waiting, pid);
            if (found == NULL)
                return;
        }

        // Update Metrics
        found->end_time = timestamp();

        // Calculate turnaround and response times for the process, then add to schedule overall
        s->total_turnaround_time += (found->end_time - found->arrival_time);
        s->total_response_time += (found->start_time - found->arrival_time); 

        // Move to finished queue
        queue_push(&s->finished, found);
        
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
