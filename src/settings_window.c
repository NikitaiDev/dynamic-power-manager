#include <gtk/gtk.h>
#include "settings_window.h"
#include "usb_manager.h"

GtkWidget *switch_widgets[NUM_DEVICES];  // Массив виджетов для переключателей

// Callback для изменения состояния устройства (Enable/Disable)
void on_device_switch_toggled(GtkSwitch *widget, gboolean state, gpointer user_data) {
    int index = GPOINTER_TO_INT(user_data);
    class_configs[index].disable = !state;  // Если переключатель включён, устройство Enable
    g_print("Device %02x is now %s\n", class_configs[index].bInterfaceClass, state ? "Enabled" : "Disabled");
}

void on_save_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Settings saved\n");
}

void on_close_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *window = GTK_WIDGET(data);
    gtk_widget_destroy(window);
}

void open_settings_window() {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *save_button, *close_button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Settings");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    for (int i = 0; i < NUM_DEVICES; i++) {
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

        GtkWidget *label = gtk_label_new(NULL);
        switch (class_configs[i].bInterfaceClass) {
            case 0x08: gtk_label_set_text(GTK_LABEL(label), "Mass Storage"); break;
            case 0x03: gtk_label_set_text(GTK_LABEL(label), "HID"); break;
            case 0x09: gtk_label_set_text(GTK_LABEL(label), "Hub"); break;
            case 0x01: gtk_label_set_text(GTK_LABEL(label), "Audio"); break;
            case 0x02: gtk_label_set_text(GTK_LABEL(label), "Communication and CDC Control"); break;
            case 0x05: gtk_label_set_text(GTK_LABEL(label), "Physical Interface Device"); break;
            case 0x06: gtk_label_set_text(GTK_LABEL(label), "Image"); break;
            case 0x07: gtk_label_set_text(GTK_LABEL(label), "Printer"); break;
            case 0x0a: gtk_label_set_text(GTK_LABEL(label), "CDC Data"); break;
            case 0x0b: gtk_label_set_text(GTK_LABEL(label), "Smart Card"); break;
            case 0x0d: gtk_label_set_text(GTK_LABEL(label), "Content Security"); break;
            case 0x0e: gtk_label_set_text(GTK_LABEL(label), "Video"); break;
            case 0x0f: gtk_label_set_text(GTK_LABEL(label), "Personal Healthcare"); break;
            case 0x10: gtk_label_set_text(GTK_LABEL(label), "Audio/Video Devices"); break;
            case 0x11: gtk_label_set_text(GTK_LABEL(label), "Billboard Device"); break;
            case 0x12: gtk_label_set_text(GTK_LABEL(label), "USB Type-C Bridge Class"); break;
            case 0x13: gtk_label_set_text(GTK_LABEL(label), "USB Bulk Display Protocol Device Class"); break;
            case 0x14: gtk_label_set_text(GTK_LABEL(label), "MCTP over USB Protocol Endpoint Device Class"); break;
            case 0x3c: gtk_label_set_text(GTK_LABEL(label), "I3C Device Class"); break;
            case 0xdc: gtk_label_set_text(GTK_LABEL(label), "Diagnostic Device"); break;
            case 0xe0: gtk_label_set_text(GTK_LABEL(label), "Wireless Controller"); break;
            case 0xef: gtk_label_set_text(GTK_LABEL(label), "Miscellaneous"); break;
            case 0xfe: gtk_label_set_text(GTK_LABEL(label), "Application Specific"); break;
            case 0xff: gtk_label_set_text(GTK_LABEL(label), "Vendor Specific"); break;
            default: gtk_label_set_text(GTK_LABEL(label), "Unknown Device"); break;
        }
        gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

        switch_widgets[i] = gtk_switch_new();
        gtk_switch_set_active(GTK_SWITCH(switch_widgets[i]), class_configs[i].disable);
        g_signal_connect(switch_widgets[i], "state-set", G_CALLBACK(on_device_switch_toggled), GINT_TO_POINTER(i));
        gtk_box_pack_end(GTK_BOX(hbox), switch_widgets[i], FALSE, FALSE, 0);
    }

    save_button = gtk_button_new_with_label("Save");
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), save_button, FALSE, FALSE, 0);

    close_button = gtk_button_new_with_label("Close");
    g_signal_connect(close_button, "clicked", G_CALLBACK(on_close_button_clicked), window);
    gtk_box_pack_start(GTK_BOX(vbox), close_button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
}
