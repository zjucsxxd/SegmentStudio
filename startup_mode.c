#include "startup_mode.h"

/* Window properties */
#define WINDOW_TITLE "Segment Studio" 
#define WINDOW_SIZE 300  /* SIZExSIZE */

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

        gtk_widget_show(window);

        smInitVbox();
        smInitText();
        smInitButtons();
        smLoop();
}

void smInitVbox()
{
        vbox = gtk_vbox_new(TRUE, 0);
        gtk_container_add(GTK_CONTAINER(window), vbox);
        gtk_widget_show(vbox);
}

/* 
 * Add a button
 * ptr - Widget pointer to bind to
 * str - label to use for button
*/
#define ADD_BUTTON(ptr, str)\
        ptr = gtk_button_new_with_label(str);\
        gtk_container_add(GTK_CONTAINER(vbox), ptr);\
        gtk_widget_show(ptr)

/* Where to place buttons on the grid */
int grid_loc = 0;

static void smInitButtons()
{
        ADD_BUTTON(view_button, "Open a segment file");
        ADD_BUTTON(capture_button, "Capture");
}

static void smInitText()
{
        select_text = gtk_label_new("Please select an option below");
        gtk_container_add(GTK_CONTAINER(vbox), select_text);
        gtk_widget_show(select_text);
}

static void smLoop()
{
        gtk_main();
}
