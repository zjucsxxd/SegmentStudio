#include "capture.h"

/* Window properties */
#define WINDOW_TITLE "Segment Studio | Capture" 
#define WINDOW_SIZE 400  /* SIZExSIZE */

/* Window */
GtkWidget * window;
GtkWidget * vbox;

/* Capture feed */
GtkWidget * capture_image;
GdkPixbuf * capture_pbuf;  /* Buffer to display image */

/* Folder path */
gchar * path;

void cpInit()
{
        gtk_init(NULL, NULL);
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

        gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
        gtk_container_set_border_width(GTK_CONTAINER(window), WINDOW_SIZE);
        gtk_window_set_policy(GTK_WINDOW(window), FALSE, FALSE, FALSE);

        /* Get directory */
        cpDirSelect();
        
        cpInitVbox();
        cpInitCapture();

        gtk_widget_show_all(window);

        gtk_main();
}

static void cpDestroy()
{
        gtk_widget_destroy(window);
        gtk_main_quit();
}

static void cpInitVbox()
{
        vbox = gtk_vbox_new(TRUE, 0);
        gtk_container_add(GTK_CONTAINER(window), vbox);
}

static void cpInitCapture()
{
        capture_image = gtk_image_new_from_pixbuf(capture_pbuf);
        gtk_container_add(GTK_CONTAINER(window), capture_image);
}

static void cpDirSelect()
{
        GtkWidget * folder_dialog = gtk_file_chooser_dialog_new(
                        "Please select a directory to save captures to",
                        GTK_WINDOW(window), 
                        GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                        GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                        NULL);

        int res = gtk_dialog_run(GTK_DIALOG(folder_dialog));


        if (res == GTK_RESPONSE_ACCEPT) {
                path = gtk_file_chooser_get_current_folder_uri(GTK_FILE_CHOOSER(folder_dialog));
                gtk_widget_destroy(folder_dialog);
        } else {
                gtk_widget_destroy(folder_dialog);
                cpDestroy();
                smInit();
        }
}
