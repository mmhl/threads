#ifndef __TIMER_H__
#define __TIMER_H__

#include <signal.h>
#include <sys/time.h>

void timer_setup(void (*alrmHandler)(int), int usec);
int timer_enable();
int timer_disable();
#endif
