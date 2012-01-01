#include "main_window.h"

void mwInit()
{
        gtk_init(NULL, NULL);
        _mw_main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        
        gtk_window_set_title(GTK_WINDOW(_mw_main_window), MAIN_WINDOW_TITLE);
        gtk_container_set_border_width(GTK_CONTAINER(_mw_main_window), MAIN_WINDOW_SIZE);

        gtk_window_set_policy(GTK_WINDOW(_mw_main_window), FALSE, FALSE, FALSE);

        gtk_widget_show(_mw_main_window);
}

void mwLoop()
{
        gtk_main();
}
