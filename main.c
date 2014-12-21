#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "thread.h"
#include "timer.h"
#include "fifo.h"

unsigned int counters[3];
static void thread_function1(void *args) {
        int i;
        for(i=0;i<100;i++) {
                counters[0]++;
                mythread_yield();
        }
        mythread_kill(1002);
        mythread_exit();
}
static void thread_function2(void *args) {
        for(;;) {
                counters[1]++;
        }
}
static void thread_function3(void *args) {
        for(;;) {
                counters[2]++;
        }
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
