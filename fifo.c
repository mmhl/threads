#include <stdio.h>
#include <unistd.h>
#include "thread.h"


#define FIFO_SIZE 64
static struct thread *fifo[FIFO_SIZE];
static int head, tail;

int queue_is_empty() {
        return head == tail;
}
int queue_is_full() {
        if(tail == FIFO_SIZE)
                return 1;
        return 0;
}

int queue_enqueue(struct thread *thr) {
        if(queue_is_full())
                return -1;
        fifo[tail++] = thr;
        return 0;
}
struct thread *queue_dequeue() {
        struct thread *t;
        if(queue_is_empty())
                return NULL;
        t = fifo[head++];
        if(head == FIFO_SIZE)
                head = 0;
        return t;

}

