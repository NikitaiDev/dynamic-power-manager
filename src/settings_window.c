#include <gtk/gtk.h>
#include "settings_window.h"

// Callback для кнопки "Save"
void on_save_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Settings saved\n");
    // Здесь можно добавить логику для сохранения настроек
}

// Callback для закрытия окна
void on_close_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *window = GTK_WIDGET(data);
    gtk_widget_destroy(window);
}

// Функция для создания и отображения окна настроек
void open_settings_window() {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *save_button, *close_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Settings");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Добавляем элементы управления
    GtkWidget *checkbox = gtk_check_button_new_with_label("Enable option 1");
    gtk_box_pack_start(GTK_BOX(vbox), checkbox, FALSE, FALSE, 0);

    GtkWidget *switch_button = gtk_switch_new();
    gtk_box_pack_start(GTK_BOX(vbox), switch_button, FALSE, FALSE, 0);

    // Добавляем кнопки "Save" и "Close"
    save_button = gtk_button_new_with_label("Save");
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), save_button, FALSE, FALSE, 0);

    close_button = gtk_button_new_with_label("Close");
    g_signal_connect(close_button, "clicked", G_CALLBACK(on_close_button_clicked), window);
    gtk_box_pack_start(GTK_BOX(vbox), close_button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
}
