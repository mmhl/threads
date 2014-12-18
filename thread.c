#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

static tid_t tid_counter = 1000;
//TEMP!!!
static int threads_count;
struct thread* thread_create(void (*thread_fn)(void *args)) {
        struct thread *new_thread;
        struct ucontext *curr;
        printf("Creating a new thread");
        new_thread = calloc(1, sizeof(struct thread));
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
        thread_pool[threads_count++] = new_thread;
        printf("Created thread number %d", threads_count);

}
