#include <stdio.h>
#include <unistd.h>
#include <libusb-1.0/libusb.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    MAX_POWER_SAVING,
    BALANCED,
    PERFORMANCE
} power_profile_t;

power_profile_t current_profile = BALANCED;

// Structure to hold device class configuration
typedef struct {
    uint8_t bInterfaceClass;
    int disable; // 1 - Disable, 0 - Do not disable
} device_class_config_t;

// Device class configurations
device_class_config_t class_configs[] = {
    {0x08, 1},  // Mass Storage - Disable
    {0x03, 0},  // HID - Do not disable
    {0x09, 0},  // Hub - Do not disable
    {0x01, 0},  // Audio - Do not disable
    {0x02, 0},  // Communication and CDC Control - Do not disable
    {0x05, 0},  // Physical Interface Device - Do not disable
    {0x06, 0},  // Image - Do not disable
    {0x07, 0},  // Printer - Do not disable
    {0x0a, 0},  // CDC Data - Do not disable
    {0x0b, 0},  // Smart Card - Do not disable
    {0x0d, 0},  // Content Security - Do not disable
    {0x0e, 0},  // Video - Do not disable
    {0x0f, 0},  // Personal Healthcare - Do not disable
    {0x10, 0},  // Audio/Video Devices - Do not disable
    {0x11, 0},  // Billboard Device - Do not disable
    {0x12, 0},  // USB Type-C Bridge Class - Do not disable
    {0x13, 0},  // USB Bulk Display Protocol Device Class - Do not disable
    {0x14, 0},  // MCTP over USB Protocol Endpoint Device Class - Do not disable
    {0x3c, 0},  // I3C Device Class - Do not disable
    {0xdc, 0},  // Diagnostic Device - Do not disable
    {0xe0, 0},  // Wireless Controller - Do not disable
    {0xef, 0},  // Miscellaneous - Do not disable
    {0xfe, 0},  // Application Specific - Do not disable
    {0xff, 0}   // Vendor Specific - Do not disable
};

// Function to find if a class should be disabled
int should_disable_class(uint8_t bInterfaceClass) {
    for (long unsigned int i = 0; i < sizeof(class_configs) / sizeof(class_configs[0]); i++) {
        if (class_configs[i].bInterfaceClass == bInterfaceClass) {
            return class_configs[i].disable;
        }
    }
    return 0;  // Default to not disabling if class not found
}

void set_power_profile(power_profile_t profile) {
    current_profile = profile;
    switch (profile) {
        case MAX_POWER_SAVING:
            printf("Power profile set to: Maximum Power Saving\n");
            break;
        case BALANCED:
            printf("Power profile set to: Balanced\n");
            break;
        case PERFORMANCE:
            printf("Power profile set to: Performance\n");
            break;
        default:
            break;
    }
}

// Function to execute lsusb and get the device description by VID and PID
void get_lsusb_description(unsigned short vid, unsigned short pid) {
    char command[128];
    snprintf(command, sizeof(command), "lsusb -d %04x:%04x", vid, pid);
    
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        printf("Failed to run lsusb command.\n");
        return;
    }
    
    char result[256];
    if (fgets(result, sizeof(result), fp) != NULL) {
        // Removing Bus and Device information from the description
        char *desc_start = strstr(result, "ID ");
        if (desc_start) {
            desc_start += 3;  // Move past "ID "
            while (*desc_start != '\0' && *desc_start != ' ') desc_start++;  // Skip VID:PID
            while (*desc_start == ' ') desc_start++;  // Skip whitespace
            printf("  Description:  %s", desc_start);
        }
    } else {
        printf("  Description: Not available\n");
    }

    pclose(fp);
}

// Function to disable a USB device (detach kernel driver or reset device)
void disable_device(libusb_device_handle *handle) {
    if (libusb_kernel_driver_active(handle, 0)) {
        if (libusb_detach_kernel_driver(handle, 0) == 0) {
            printf("  Device disabled by detaching kernel driver.\n");
        } else {
            printf("  Failed to detach kernel driver.\n");
        }
    } else {
        printf("  Device reset.\n");
        libusb_reset_device(handle);
    }
}

void print_device_descriptors_and_disable_mass_storage() {
    libusb_context *ctx = NULL;
    libusb_device **devs;
    ssize_t cnt;
    int i;

    if (libusb_init(&ctx) < 0) {
        fprintf(stderr, "Failed to initialize libusb.\n");
        return;
    }

    cnt = libusb_get_device_list(ctx, &devs);
    if (cnt < 0) {
        fprintf(stderr, "Failed to get device list.\n");
        libusb_exit(ctx);
        return;
    }

    for (i = 0; i < cnt; i++) {
        libusb_device *dev = devs[i];
        struct libusb_device_descriptor desc;
        if (libusb_get_device_descriptor(dev, &desc) < 0) {
            fprintf(stderr, "Failed to get device descriptor.\n");
            continue;
        }

        printf("Device %d:  VID: %04x, PID: %04x\n", i, desc.idVendor, desc.idProduct);

        // Get lsusb description
        get_lsusb_description(desc.idVendor, desc.idProduct);

        int class_identified = 0;  // Flag to check if class info has been identified

        struct libusb_config_descriptor *config;
        if (libusb_get_config_descriptor(dev, 0, &config) == 0) {
            for (int j = 0; j < config->bNumInterfaces; j++) {
                const struct libusb_interface *interface = &config->interface[j];
                for (int k = 0; k < interface->num_altsetting; k++) {
                    const struct libusb_interface_descriptor *interface_desc = &interface->altsetting[k];

                    if (!class_identified) {
                        printf("  Interface Class: %02x", interface_desc->bInterfaceClass);

                        // Check if the class should be disabled
                        if (should_disable_class(interface_desc->bInterfaceClass)) {
                            printf(" - Mass Storage (Disabling)\n");

                            // Disable the device if it's Mass Storage
                            libusb_device_handle *handle;
                            if (libusb_open(dev, &handle) == 0) {
                                disable_device(handle);
                                libusb_close(handle);
                            } else {
                                printf("  Failed to open device for disabling.\n");
                            }
                        } else {
                            switch (interface_desc->bInterfaceClass) {
                                case 0x01:
                                    printf(" - Audio\n");
                                    break;
                                case 0x02:
                                    printf(" - Communication and CDC Control\n");
                                    break;
                                case 0x03:
                                    switch (interface_desc->bInterfaceSubClass) {
                                        case 0x01:
                                            printf(" - HID Keyboard\n");
                                            break;
                                        case 0x02:
                                            printf(" - HID Mouse\n");
                                            break;
                                        default:
                                            printf(" - HID Device\n");
                                            break;
                                    }
                                    break;
                                case 0x05:
                                    printf(" - Physical Interface Device\n");
                                    break;
                                case 0x06:
                                    printf(" - Image\n");
                                    break;
                                case 0x07:
                                    printf(" - Printer\n");
                                    break;
                                case 0x09:
                                    printf(" - Hub\n");
                                    break;
                                case 0x0a:
                                    printf(" - CDC Data\n");
                                    break;
                                case 0x0b:
                                    printf(" - Smart Card\n");
                                    break;
                                case 0x0d:
                                    printf(" - Content Security\n");
                                    break;
                                case 0x0e:
                                    printf(" - Video\n");
                                    break;
                                case 0x0f:
                                    printf(" - Personal Healthcare\n");
                                    break;
                                case 0x10:
                                    printf(" - Audio/Video Devices\n");
                                    break;
                                case 0x11:
                                    printf(" - Billboard Device\n");
                                    break;
                                case 0x12:
                                    printf(" - USB Type-C Bridge Class\n");
                                    break;
                                case 0x13:
                                    printf(" - USB Bulk Display Protocol Device Class\n");
                                    break;
                                case 0x14:
                                    printf(" - MCTP over USB Protocol Endpoint Device Class\n");
                                    break;
                                case 0x3c:
                                    printf(" - I3C Device Class\n");
                                    break;
                                case 0xdc:
                                    printf(" - Diagnostic Device\n");
                                    break;
                                case 0xe0:
                                    printf(" - Wireless Controller\n");
                                    break;
                                case 0xef:
                                    printf(" - Miscellaneous\n");
                                    break;
                                case 0xfe:
                                    printf(" - Application Specific\n");
                                    break;
                                case 0xff:
                                    printf(" - Vendor Specific\n");
                                    break;
                                default:
                                    printf(" - Unknown Class\n");
                                    break;
                            }
                        }

                        printf("\n");
                        class_identified = 1;
                    }
                }
            }
            libusb_free_config_descriptor(config);
        } else {
            printf("  Failed to get configuration descriptor.\n");
        }
        printf("\n");
    }

    libusb_free_device_list(devs, 1);
    libusb_exit(ctx);
}

int main() {
    printf("Starting dynamic power manager\n");

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
