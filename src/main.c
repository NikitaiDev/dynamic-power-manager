#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "usb_manager.h"
#include "tray_icon.h" 

int main() {
    printf("Starting dynamic power manager\n");

    // Запускаем иконку в трее
    start_tray_icon();

    set_power_profile(MAX_POWER_SAVING);
    print_device_descriptors_and_disable_mass_storage();

    while (1) {
        switch (current_profile) {
            case MAX_POWER_SAVING:
                printf("Disabling unused devices\n");
                break;
            case BALANCED:
                printf("Balancing power consumption\n");
                break;
            case PERFORMANCE:
                printf("Running in performance mode\n");
                break;
        }

        sleep(60);
    }

    return 0;
}
