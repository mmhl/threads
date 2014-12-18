#include <stdio.h>
#include <unistd.h>
#include "thread.h"
#include "timer.h"


static int current_task = 0;
void scheduler(int signum) {
        int next_task = (current_task+1)%2;
        struct thread *current;
        struct thread *next;
        current = thread_pool[current_task++];
        next = thread_pool[next_task];
        printf("Scheduler\n");
        swapcontext(current->context, next->context);

}
static void thread_function1(void *args) {
        for(;;) {
                fprintf(stderr,"1");
                sleep(1);
        }
}
static void thread_function2(void *args) {
        for(;;) {
                fprintf(stderr,"2");
                sleep(1);
        }
}
int main() {
        //Setup timer, this effectively start scheduler
        mythread_start(thread_function1, NULL);
        mythread_start(thread_function2, NULL);
        timer_setup(scheduler, 500000);
        //Should not return;
        for(;;) {
        }
        return 0;

}
