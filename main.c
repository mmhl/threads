#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "thread.h"
#include "timer.h"
#include "fifo.h"


static void thread_function1(void *args) {
        int x = 10;
        for(;;) {
                fprintf(stdout,"%d\n", x++);
        }
}
static void thread_function2(void *args) {
        int x = 20000;
        for(;;) {
                fprintf(stdout,"%d\n", x++);
        }
}
int main() {
        //Setup timer, this effectively start scheduler
        //This is for initial context

        sched_init();
        mythread_start(thread_function1, NULL);
        mythread_start(thread_function2, NULL);
        timer_setup(sig_timer_handler, 100000);
        //Should not return;
        for(;;) {
        }
        return 0;

}
