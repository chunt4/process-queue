/* queue.c: PQSH Queue */

#include "pqsh/macros.h"
#include "pqsh/queue.h"

#include <assert.h>

/**
 * Push process to back of queue.
 * @param q     Pointer to Queue structure.
 **/
void        queue_push(Queue *q, Process *p) {
    /* TODO: Implement */
    if (q->size == 0){
        q->head = p;
        q->tail = p;
        q->size++;
    }
    else{
        q->tail->next=p;
        q->tail=p;
        q->size++;
    }
}

/**
 * Pop process from front of queue.
 * @param q     Pointer to Queue structure.
 * @return  Process from front of queue.
 **/
Process *   queue_pop(Queue *q) {
    /* TODO: Implement */
    Process *temp = q->head;
    q->head = q->head->next;
    q->size--;
    return temp;
}

/**
 * Remove and return process with specified pid.
 * @param q     Pointer to Queue structure.
 * @param pid   Pid of process to return.
 * @return  Process from Queue with specified pid.
 **/
Process *   queue_remove(Queue *q, pid_t pid) {
    /* TODO: Implement */
    if (q->size == 0)
        return NULL;
    
    if (q->head->pid == pid)
        return queue_pop(q);

    for (Process *p = q->head; q->tail; p = p->next){
        if(pid == p->pid){
            q->prev->next = p->next;
            q->size--;
            return p;
        }
    }
    return NULL;
}

/**
 * Dump the contents of the Queue to the specified stream.
 * @param q     Queue structure.
 * @param fs    Output file stream.
 **/
void        queue_dump(Queue *q, FILE *fs) {
    fprintf(fs, "%6s %-30s %-13s %-13s %-13s\n", 
                "PID", "COMMAND", "ARRIVAL", "START", "END");
    /* TODO: Display information for each item in Queue. */
    for (Process *p = q->head; q->tail; p=p->next){
        fprintf(fs, "%6s %-30s %-13s %-13s %-13s\n", p->pid, p->command, p->arrival_time, p->start_time, p->end_time);
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
