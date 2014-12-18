#ifndef __CONFIG_THREAD_H
#define __CONFIG_THREAD_H
#include <ucontext.h>

#define STACK_SIZE 4096
typedef unsigned int tid_t;
struct thread {
        struct ucontext *context;
        tid_t tid;
        unsigned char stack[STACK_SIZE];
        size_t stack_size;
        void (*thread_fn)(void *args);
};


struct thread *thread_pool[2];
struct thread* thread_create(void (*thread_fn)(void *args));
void mythread_start(void (*thread_fn)(void *args), void *args);
int mythread_exit();
int mythread_kill();
#endif
