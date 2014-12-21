#ifndef __FIFO_H__
#define __FIFO_H__
#include "thread.h"
#define FIFO_SIZE 64
struct fifo {
        struct thread *queue[FIFO_SIZE];
        int head;
        int tail;
        int elems;
};
int queue_is_empty(struct fifo *q);
int queue_is_full(struct fifo *q);
int queue_enqueue(struct thread *thr, struct fifo *q);
struct thread *queue_dequeue(struct fifo *q);
struct thread *queue_find(tid_t tid, struct fifo *q);
#endif
