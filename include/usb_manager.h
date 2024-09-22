#ifndef USB_MANAGER_H
#define USB_MANAGER_H

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <string.h>

#define NUM_DEVICES 24  

typedef enum {
    MAX_POWER_SAVING,
    BALANCED,
    PERFORMANCE
} power_profile_t;

extern power_profile_t current_profile;

// Structure to hold device class configuration
typedef struct {
    uint8_t bInterfaceClass;
    int disable; // 1 - Disable, 0 - Do not disable
} device_class_config_t;

// Device class configurations
extern device_class_config_t class_configs[];

void set_power_profile(power_profile_t profile);
void print_device_descriptors_and_disable_mass_storage();

#endif