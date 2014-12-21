#include <stdio.h>
#include <unistd.h>
#include "thread.h"
#include "fifo.h"

int queue_is_empty(struct fifo *q) {
        return q->head == q->tail;
}
int queue_is_full(struct fifo *q) {
        if(q->tail == FIFO_SIZE)
                return 1;
        return 0;
}

int queue_enqueue(struct thread *thr,struct fifo *q) {
        if(queue_is_full(q))
                return -1;
        q->queue[q->tail++] = thr;
        return 0;
}
struct thread *queue_dequeue(struct fifo *q) {
        struct thread *t;
        if(queue_is_empty(q))
                return NULL;
        t = q->queue[q->head];
        q->queue[q->head++]= NULL;
        if(q->head == FIFO_SIZE)
                q->head = 0;
        return t;

}

struct thread *queue_find(tid_t tid, struct fifo *q) {
        int i;
        for(i = q->head; i != q->tail; i++) {
                struct thread *t = q->queue[i];
                if(t->tid == tid)
                        return t;
        }
        return NULL;
}
