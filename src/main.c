#include "tray_icon.h"
#include "usb_manager.h"
#include <pthread.h>
#include <stdio.h>

// Основная функция
int main() {
    if (pthread_create(&task_thread, NULL, background_task, NULL)) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    start_tray_icon();

    stop_tray_icon();

    return 0;
}
