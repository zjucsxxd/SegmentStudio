#include "startup_mode.h"

/* Window properties */
#define WINDOW_TITLE "Segment Studio" 
#define WINDOW_SIZE 100  /* SIZExSIZE */

/* Main window */
GtkWidget *window;
GtkWidget *vbox;

/* Buttons */
GtkWidget *capture_button;
GtkWidget *view_button;

/* Text */
GtkWidget *select_text;

void smInit()
{
        gtk_init(NULL, NULL);
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

        gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
        gtk_container_set_border_width(GTK_CONTAINER(window), WINDOW_SIZE);
        gtk_window_set_policy(GTK_WINDOW(window), FALSE, FALSE, FALSE);

        smInitVbox();
        smInitText();
        smInitButtons();

        gtk_widget_show_all(window);

        gtk_main();
}

static void smDestroy()
{
        gtk_widget_destroy(window);
        gtk_main_quit();
}

static void smInitVbox()
{
        vbox = gtk_vbox_new(TRUE, 0);
        gtk_container_add(GTK_CONTAINER(window), vbox);
}

/* 
 * Add a button
 * ptr - Widget pointer to bind to
 * str - label to use for button
*/
#define ADD_BUTTON(ptr, str)\
        ptr = gtk_button_new_with_label(str);\
        gtk_container_add(GTK_CONTAINER(vbox), ptr);\
        g_signal_connect(ptr, "clicked", G_CALLBACK(smButtonCallback), NULL);\
        gtk_widget_show(ptr)

static void smInitButtons()
{
        ADD_BUTTON(view_button, "Open a segment file");
        ADD_BUTTON(capture_button, "Capture");
}

static void smInitText()
{
        select_text = gtk_label_new("Please select an option below");
        gtk_container_add(GTK_CONTAINER(vbox), select_text);
}

static void smButtonCallback(GtkWidget *widget, gpointer data)
{
        smDestroy();

        if (widget == capture_button)
                cpInit();
}
