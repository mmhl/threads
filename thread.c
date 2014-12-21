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
        new_thread->state= RUNNABLE;

}
//Thread voluntarily kills itself. Should not return
int mythread_exit() {
        current->state = FINISHED;
        schedule();
        return 0; // silence the compiler
}

int mythread_kill(tid_t tid) {
        printf("%d tries to kill %d\n", current->tid, tid);
        struct thread *t = queue_find(tid, &thread_queue);
        if(t != NULL) {
                printf("FOUND %d\n", tid);
                t->state = FINISHED;
                return 0;
        }
        return -1;
}

void mythread_yield() {
        printf("%d yields\n", current->tid);
        schedule();
}


void sig_timer_handler(int signum) {
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
        preempt_disable();
        struct thread *prev= current;
        struct thread *next = queue_dequeue(&thread_queue);

        if(next == NULL ) {
                printf("No more tasks\n");
                next = &sched;
        }
        else if(next->state == FINISHED) {
                printf("Cleanup stale stask %d\n", next->tid);
                next = &sched;
        }

        if(current->state != FINISHED && current != &sched) {
                printf("task %d still has some work to do\n", current->tid);
                queue_enqueue(current,&thread_queue);
        }
        else if(current->state == FINISHED) {
                printf("task %d finished\n", current->tid);
                __thread_cleanup(current);
                current = NULL;
        }
        prev = current;
        current = next;

        preempt_enable();
        if(prev == NULL) {
                printf("Set context to: %d\n", next->tid);
                setcontext(next->context);
        }
        else {
                printf("Set context from %d to: %d\n", prev->tid, next->tid);
                swapcontext(prev->context, next->context);
        }
}


void preempt_disable() {
        timer_disable();
}
void preempt_enable() {
        timer_enable();
}
