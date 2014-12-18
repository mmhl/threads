#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "fifo.h"

struct thread *current;
struct thread sched;

static tid_t tid_counter = 1000;
struct thread* thread_create(void (*thread_fn)(void *args)) {
        struct thread *new_thread;
        struct ucontext *curr;
        printf("Creating a new thread");
        new_thread = calloc(1, sizeof(struct thread));
        curr = malloc(sizeof(struct ucontext));
        //Get current context 
        getcontext(curr); 
        
        new_thread->context = curr;
        new_thread->thread_fn = thread_fn;
        return new_thread;
}
void mythread_start(void (*thread_fn)(), void *args) {
        struct thread *new_thread;
        struct ucontext *curr;
        printf("Creating a new thread");
        curr = malloc(sizeof(struct ucontext));
        new_thread = calloc(1, sizeof(struct thread));
        new_thread->stack_size = STACK_SIZE;
        //Get current context 
        getcontext(curr); 
        curr->uc_stack.ss_sp = new_thread->stack;
        curr->uc_stack.ss_size = new_thread->stack_size;
        curr->uc_stack.ss_flags = 0;

        new_thread->context = curr;
        new_thread->thread_fn = thread_fn;
        new_thread->tid = ++tid_counter;
        makecontext(new_thread->context, thread_fn, 1, args);
        queue_enqueue(new_thread);
        printf("Created thread with tid %d\n", new_thread->tid);

}


void sig_timer_handler(int signum) {
        //TODO: Disable timers!
        schedule();
}

//Create minimal context to start up
void sched_init() {
        sched.context = malloc(sizeof(struct ucontext));
        getcontext(sched.context); // sched context
        current = &sched;
}
//scheduler main function
void schedule() {
        struct thread *next = queue_dequeue();
        struct thread *prev = current;
        queue_enqueue(next); // if thread is not finished yet: TODO, add it to the end of the list
        fprintf(stdout, "Scheduling thread with tid %d\n", next->tid);
        current = next;
        //enqueue current running thread to the end of the queue
        swapcontext(prev->context, next->context);
}
