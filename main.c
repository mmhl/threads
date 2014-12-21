#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "thread.h"
#include "timer.h"
#include "fifo.h"
#include "mutex.h"

mutex_t out_mutex;
unsigned int counters[3];
static void thread_function1(void *args) {
        int i;
        mutex_lock(&out_mutex);
        for(i=0;i<10;i++) {
                printf("Hello %d from thread one\n", i);
                usleep(500000);
        }
        mutex_unlock(&out_mutex);
        mythread_kill(1002);
        mythread_exit();
}
static void thread_function2(void *args) {
        int i;
        mutex_lock(&out_mutex);
        for(i=10;i < 20; i++) {
                printf("Hello %d from thread two\n", i);
                usleep(750000);
        }
        mutex_unlock(&out_mutex);
}
static void thread_function3(void *args) {
        int i;
        mutex_lock(&out_mutex);
        for(i=30;i < 40; i++) {
                printf("Hello %d from thread three\n", i);
                usleep(550000);
        }
        mutex_unlock(&out_mutex);
}

int main() {
        sched_init();
        mythread_start(thread_function1, NULL);
        mythread_start(thread_function2, NULL);
        mythread_start(thread_function3, NULL);
        timer_setup(sig_timer_handler, 25000);
        while(1) ;
        return 0;

}
