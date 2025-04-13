#include "my_timer.h"

MY_TIMER* create_timer() {
    MY_TIMER *timer = SDL_malloc(sizeof(MY_TIMER));
    timer->begin_time = SDL_GetTicks();
    timer->thread_clock;
    timer->time_elapsed = 0;
    timer->start_timer = 0;
}

void destroy_timer(MY_TIMER *timer) {
    SDL_free(timer);
}

void* count_down(void *atribute) {
    MY_TIMER *timer = (MY_TIMER*)atribute;
    int m_sec = 0;
    int old_sec = 0;
    while (timer->start_timer) {
        Uint32 now = SDL_GetTicks();
        int m_sec = (now - timer->begin_time)/ 1000;
        if (m_sec > old_sec) {
            old_sec = m_sec;
            //SDL_Log("seconds: %d\n", m_sec);
            timer->time_elapsed = old_sec;
        }
    }
    return NULL;
}

int reset_timer(MY_TIMER *timer) {
    timer->start_timer = 0;
    pthread_join(timer->thread_clock, NULL);
    timer->time_elapsed = 0;
}

int start_timer(MY_TIMER *timer) {
    timer->start_timer = 1;
    timer->time_elapsed = 0;
    timer->begin_time = SDL_GetTicks();
    pthread_create(&timer->thread_clock, NULL, count_down, (void*)timer);

}