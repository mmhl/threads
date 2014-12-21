#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "thread.h"
#include "timer.h"
#include "fifo.h"


static void thread_function1(void *args) {
        int i= 0;
        for(i=0;i<5;i++) {
                fprintf(stdout,"%d\n", i);
                sleep(1);
        }
        mythread_kill(1002);
        mythread_exit();
}
static void thread_function2(void *args) {
        int x = 20000;
        for(;;) {
                fprintf(stdout,"%d\n", x++);
                sleep(1);
                if(x==20015) 
                        return;
        }
}

int main() {
        //This is for initial context
        sched_init();
        mythread_start(thread_function1, NULL);
        mythread_start(thread_function2, NULL);
        timer_setup(sig_timer_handler, 250000);
        schedule();
        while(1);
        return 0;

}
