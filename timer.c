#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "timer.h"

static struct sigaction sa;

//Setup 
void timer_setup(void (*alrmHandler)(int), int usec) {
        struct itimerval it;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sa.sa_handler = alrmHandler;
        if(sigaction(SIGALRM, &sa, NULL)==-1) {
                printf("sigaction\n");
                exit(-1);
        }
               
        it.it_value.tv_sec = 0;
        it.it_value.tv_usec= usec;
        it.it_interval.tv_sec= 0;
        it.it_interval.tv_usec= usec;
        if(setitimer(ITIMER_REAL, &it, NULL)==-1) {
                printf("setittimer %s\n", strerror(errno));
                exit(-1);
        }
}

