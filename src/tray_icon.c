#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>
#include <pthread.h>
#include "tray_icon.h"
#include "usb_manager.h"
#include "settings_window.h"

pthread_t task_thread;

void on_settings_activated(GtkMenuItem *item, gpointer user_data) {
    open_settings_window();
}

void on_quit_activated(GtkMenuItem *item, gpointer user_data) {
    pthread_cancel(task_thread);
    gtk_main_quit();
}

void start_tray_icon() {
    int argc = 0;
    char **argv = NULL;
    gtk_init(&argc, &argv);

    AppIndicator *indicator = app_indicator_new("power_manager", "battery", APP_INDICATOR_CATEGORY_SYSTEM_SERVICES);
    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);

    GtkWidget *menu = gtk_menu_new();

    GtkWidget *settings_item = gtk_menu_item_new_with_label("Settings");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), settings_item);
    g_signal_connect(settings_item, "activate", G_CALLBACK(on_settings_activated), NULL);

    GtkWidget *quit_item = gtk_menu_item_new_with_label("Quit");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), quit_item);
    g_signal_connect(quit_item, "activate", G_CALLBACK(on_quit_activated), NULL);

    gtk_widget_show_all(menu);
    app_indicator_set_menu(indicator, GTK_MENU(menu));

    gtk_main();
}

void *background_task(void *arg) {
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
    return NULL;
}

void stop_tray_icon() {
    pthread_cancel(task_thread);
}
