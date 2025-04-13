#ifndef MY_TIMER_H
#define MY_TIMER_H

#include <time.h>


typedef struct _MY_TIMER {
    Uint32 begin_time;
    int time_elapsed;
    pthread_t thread_clock;
    int start_timer;

} MY_TIMER;


MY_TIMER* create_timer();
void destroy_timer(MY_TIMER *timer);
int reset_timer(MY_TIMER *timer);
int start_timer(MY_TIMER *timer);
void* count_down(void *timer);


#endif