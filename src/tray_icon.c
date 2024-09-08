#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>
#include "tray_icon.h"

void on_settings_activated(GtkMenuItem *item, gpointer user_data) {
    g_print("Settings selected\n");
}

void on_quit_activated(GtkMenuItem *item, gpointer user_data) {
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
