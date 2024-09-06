#include <stdio.h>
#include <unistd.h>
#include <libusb-1.0/libusb.h>

typedef enum {
    MAX_POWER_SAVING,
    BALANCED,
    PERFORMANCE
} power_profile_t;

power_profile_t current_profile = BALANCED;

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

void print_device_descriptors() {
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

        printf("Device %d:\n", i);
        printf("  VID: %04x, PID: %04x\n", desc.idVendor, desc.idProduct);

        int class_identified = 0;  // Flag to check if class info has been identified

        struct libusb_config_descriptor *config;
        if (libusb_get_config_descriptor(dev, 0, &config) == 0) {
            for (int j = 0; j < config->bNumInterfaces; j++) {
                const struct libusb_interface *interface = &config->interface[j];
                for (int k = 0; k < interface->num_altsetting; k++) {
                    const struct libusb_interface_descriptor *interface_desc = &interface->altsetting[k];

                    if (!class_identified) {
                        printf("  Interface Class: %02x", interface_desc->bInterfaceClass);

                        switch (interface_desc->bInterfaceClass) {
                            case 0x01:
                                printf(" - Audio\n");
                                class_identified = 1;
                                break;
                            case 0x02:
                                printf(" - Communication and CDC Control\n");
                                class_identified = 1;
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
                                class_identified = 1;
                                break;
                            case 0x05:
                                printf(" - Physical Interface Device\n");
                                class_identified = 1;
                                break;
                            case 0x06:
                                printf(" - Image\n");
                                class_identified = 1;
                                break;
                            case 0x07:
                                printf(" - Printer\n");
                                class_identified = 1;
                                break;
                            case 0x08:
                                printf(" - Mass Storage\n");
                                class_identified = 1;
                                break;
                            case 0x09:
                                printf(" - Hub\n");
                                class_identified = 1;
                                break;
                            case 0x0a:
                                printf(" - CDC Data\n");
                                class_identified = 1;
                                break;
                            case 0x0b:
                                printf(" - Smart Card\n");
                                class_identified = 1;
                                break;
                            case 0x0d:
                                printf(" - Content Security\n");
                                class_identified = 1;
                                break;
                            case 0x0e:
                                printf(" - Video\n");
                                class_identified = 1;
                                break;
                            case 0x0f:
                                printf(" - Personal Healthcare\n");
                                class_identified = 1;
                                break;
                            case 0x10:
                                printf(" - Audio/Video Devices\n");
                                class_identified = 1;
                                break;
                            case 0x11:
                                printf(" - Billboard Device\n");
                                class_identified = 1;
                                break;
                            case 0x12:
                                printf(" - USB Type-C Bridge Class\n");
                                class_identified = 1;
                                break;
                            case 0x13:
                                printf(" - USB Bulk Display Protocol Device Class\n");
                                class_identified = 1;
                                break;
                            case 0x14:
                                printf(" - MCTP over USB Protocol Endpoint Device Class\n");
                                class_identified = 1;
                                break;
                            case 0x3c:
                                printf(" - I3C Device Class\n");
                                class_identified = 1;
                                break;
                            case 0xdc:
                                printf(" - Diagnostic Device\n");
                                class_identified = 1;
                                break;
                            case 0xe0:
                                printf(" - Wireless Controller\n");
                                class_identified = 1;
                                break;
                            case 0xef:
                                printf(" - Miscellaneous\n");
                                class_identified = 1;
                                break;
                            case 0xfe:
                                printf(" - Application Specific\n");
                                class_identified = 1;
                                break;
                            case 0xff:
                                printf(" - Vendor Specific\n");
                                class_identified = 1;
                                break;
                            default:
                                printf(" - Unknown Class\n");
                                break;
                        }

                        if (interface_desc->bInterfaceSubClass != 0) {
                            printf(", SubClass: %02x", interface_desc->bInterfaceSubClass);
                        }
                        printf("\n");
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
    print_device_descriptors();

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
