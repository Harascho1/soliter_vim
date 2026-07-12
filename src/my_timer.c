#include "my_timer.h"
#include "SDL3/SDL_events.h"

static int m_fps = 60;

MY_TIMER* create_timer() {
  MY_TIMER* timer = SDL_malloc(sizeof(MY_TIMER));
  timer->begin_time = 0;
  timer->thread_clock;
  timer->time_elapsed = 0;
  timer->start_timer = 0;
  return timer;
}

void destroy_timer(MY_TIMER* timer) {
#ifdef _WIN32
// CloseHandle(timer->thread_clock);
#endif
  SDL_free(timer);
}

void* count_down(void* atribute) {
  MY_TIMER* timer = (MY_TIMER*)atribute;
  int m_sec = 0;
  int old_sec = 0;
  while (timer->start_timer) {
    Uint32 now = SDL_GetTicks();
    int m_sec = (now - timer->begin_time) / 1000;
    if (m_sec > old_sec) {
      // SDL_Log("idalje curka\n");
      old_sec = m_sec;
      timer->time_elapsed = old_sec;
      SDL_Event user;
      user.type = SDL_EVENT_PRIVATE0;
      SDL_PushEvent(&user);
    }
    SDL_Delay(10);
  }
  return NULL;
}

int stop_timer(MY_TIMER* timer) {
  if (timer->start_timer == 0) {
    return 1;
  }
  timer->start_timer = 0;
#ifdef _WIN32
  WaitForSingleObject(timer->thread_clock, INFINITE);
  CloseHandle(timer->thread_clock);
#else
  pthread_join(timer->thread_clock, NULL);
#endif
  return 1;
}

int reset_timer(MY_TIMER* timer) {
  timer->time_elapsed = 0;
  if (timer->start_timer == 0) {
    return 1;
  }
  timer->start_timer = 0;
#ifdef _WIN32
  WaitForSingleObject(timer->thread_clock, INFINITE);
  CloseHandle(timer->thread_clock);
#else
  pthread_join(timer->thread_clock, NULL);
#endif
  return 1;
}

int start_timer(MY_TIMER* timer) {
  timer->start_timer = 1;
  timer->time_elapsed = 0;
  timer->begin_time = SDL_GetTicks();
#ifdef _WIN32
  timer->thread_clock =
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)count_down, (void*)timer, 0, NULL);
#else
  pthread_create(&timer->thread_clock, NULL, count_down, (void*)timer);
#endif
  return 1;
}
