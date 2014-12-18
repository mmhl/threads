#ifndef __FIFO_H__
#define __FIFO_H__
#include "thread.h"
int queue_is_empty();
int queue_is_full();
int queue_enqueue(struct thread *thr);
struct thread *queue_dequeue();
#endif
