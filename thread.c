#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "timer.h"
#include "fifo.h"

struct thread *current;
struct thread sched;
struct fifo thread_queue;

static tid_t tid_counter = 1000;

static void __thread_start(void (*fn)(void *), void *args) {
        fn(args);
        current->state = FINISHED;
}

static void __thread_cleanup(struct thread *th) {
        printf("Cleaning up after thread %d\n", th->tid);
        free(th->context); 
        free(th);
}

void mythread_start(void (*thread_fn)(), void *args) {
        struct thread *new_thread;
        struct ucontext *curr;
        curr = malloc(sizeof(struct ucontext));
        new_thread = calloc(1, sizeof(struct thread));
        new_thread->stack_size = STACK_SIZE;

        getcontext(curr); 
        curr->uc_stack.ss_sp = new_thread->stack;
        curr->uc_stack.ss_size = new_thread->stack_size;
        curr->uc_stack.ss_flags = 0;
        curr->uc_link = sched.context;

        new_thread->context = curr;
        new_thread->thread_fn = thread_fn;
        new_thread->tid = ++tid_counter;
        makecontext(new_thread->context, 
                        (void(*)())__thread_start, 2, new_thread->thread_fn, args);
        queue_enqueue(new_thread, &thread_queue);
        printf("Created thread with tid %d\n", new_thread->tid);
        new_thread->state= RUNNABLE;

}
//Thread voluntarily kills itself
int mythread_exit() {
        printf("Thread kill: %d \n", current->tid);
        current->state = FINISHED;
        schedule();
}

int mythread_kill(tid_t tid) {
        struct thread *t = queue_find(tid, &thread_queue);
        if(t != NULL) {
                printf("Found thread with tid %d, killing\n", t->tid);
                t->state = FINISHED;
                return 0;
        }
        return -1;
}


void sig_timer_handler(int signum) {
        printf("timer\n");
        schedule();
}

//Create minimal context to start up
void sched_init() {
        sched.stack_size = STACK_SIZE;
        sched.context = malloc(sizeof(struct ucontext));
        getcontext(sched.context); 
        sched.tid = 1;
        sched.context->uc_stack.ss_sp = sched.stack;
        sched.context->uc_stack.ss_size = sched.stack_size;
        sched.context->uc_stack.ss_flags = 0;
        sched.thread_fn = schedule;
        makecontext(sched.context, schedule, 0);
        current = &sched;
}
//scheduler main function
void schedule() {
        printf("Schedule\n");
        preempt_disable();
        struct thread *prev= current;
        struct thread *next = queue_dequeue(&thread_queue);
        if(next == NULL || next->state == FINISHED)  // if no new task is available, use sched
                next = &sched;
        printf("prev: %d next: %d\n", prev->tid, next->tid);
        if(current->state != FINISHED && current->tid != 1) {
                printf("Enqueue task %d\n", current->tid);
                queue_enqueue(current,&thread_queue);
        }
        else {
                printf("thread %d is finished\n", current->tid);
        }
        prev = current;
        //enqueue current running thread to the end of the queue
        preempt_enable();
        printf("current: %d, %d to %d\n", current->tid, prev->tid, next->tid);
        current = next;
        swapcontext(prev->context, next->context);
}


void preempt_disable() {
        timer_disable();
}
void preempt_enable() {
        timer_enable();
}
