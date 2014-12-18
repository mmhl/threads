#ifndef __CONFIG_THREAD_H
#define __CONFIG_THREAD_H
#include <ucontext.h>

#define STACK_SIZE (32*1024)
typedef unsigned int tid_t;
struct thread {
        struct ucontext *context;
        tid_t tid;
        unsigned char stack[STACK_SIZE];
        size_t stack_size;
        void (*thread_fn)(void *args);
};


struct thread *thread_create(void (*thread_fn)(void *args));
void mythread_start(void (*thread_fn)(void *args), void *args);
int mythread_exit();
int mythread_kill();

//TODO: Some timer disable/enable code
//Schedule
void sig_timer_handler(int signum);
void preempt_disable();
void preempt_enable();

extern struct thread *current; // Currently running thread
struct thread *get_next_thread(); //get next thread from the queue
void sched_init();
void schedule(); 



#endif
