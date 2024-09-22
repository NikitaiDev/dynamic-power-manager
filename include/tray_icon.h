#ifndef TRAY_ICON_H
#define TRAY_ICON_H

#include <pthread.h>

extern pthread_t task_thread;

void start_tray_icon();
void stop_tray_icon();

void *background_task(void *arg);

#endif
