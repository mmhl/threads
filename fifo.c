#include <stdio.h>
#include <unistd.h>
#include "thread.h"
#include "fifo.h"

int queue_is_empty(struct fifo *q) {
        return q->elems == 0; 
}
int queue_is_full(struct fifo *q) {
        if(q->elems == FIFO_SIZE)
                return 1;
        return 0;
}

int queue_enqueue(struct thread *thr,struct fifo *q) {
        if(queue_is_full(q))
                return -1;
        q->queue[q->tail] = thr;
        q->tail = (q->tail+1)%(FIFO_SIZE);
        q->elems++;
        return 0;
}
struct thread *queue_dequeue(struct fifo *q) {
        struct thread *t;
        if(queue_is_empty(q))
                return NULL;
        t = q->queue[q->head];
        q->queue[q->head]= NULL;
        q->head = (q->head+1)%(FIFO_SIZE);
        q->elems--;
        return t;

}

struct thread *queue_find(tid_t tid, struct fifo *q) {
        int i;
        for(i = 0; i != FIFO_SIZE; i++) {
                struct thread *t = q->queue[i];
                if(t != NULL && t->tid == tid)
                        return t;
        }
        return NULL;
}
