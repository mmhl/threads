#ifndef __MUTEX_H__
#define __MUTEX_H__
#include <stdio.h>
#include "thread.h"

//GCC specific __sync_bool_compare_and_swap
typedef volatile int mutex_t;
static inline void mutex_lock(mutex_t *lock) {
        while(!__sync_bool_compare_and_swap(lock, 0 ,1)) {
                //printf("MUTEX: Wait\n");
                schedule();
        }
        //printf("MUTEX: Obtained\n");
}
static inline void  mutex_unlock(mutex_t *lock) {
        __sync_bool_compare_and_swap(lock, 1, 0);
        //printf("MUTEX: Released\n");
}

#endif
