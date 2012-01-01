#include "capture.h"

/* Window properties */
#define WINDOW_TITLE "Segment Studio | Capture" 
#define WINDOW_SIZE 400  /* SIZExSIZE */

/* GL viewport context properties */
#define GLC_TITLE "Viewer"
#define GLC_WIDTH 640
#define GLC_HEIGHT 480

/* All side threads are bound to this */
int is_running = FALSE;

/* Window */
GtkWidget * window;
GtkWidget * vbox;

/* Folder path */
gchar * path;

/* GL view */
pthread_t glc_thread;

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
        cpInitGLWindow();

        gtk_widget_show_all(window);

        gtk_main();
}

static void cpInitGLWindow()
{
        glfwInit();

        glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);

        glfwOpenWindow(GLC_WIDTH,
                       GLC_HEIGHT,
                       8, /* 24-bit RGB */
                       8,
                       8,
                       0,
                       0, /* Depth bits */
                       0, /* Stencil bits */
                       GLFW_WINDOW);

        glfwSetWindowTitle(GLC_TITLE);
        glfwPollEvents();
}

static void cpDestroy()
{
        /* Kill all threads */
        is_running = FALSE;

        pthread_join(glc_thread, NULL); 
        glfwTerminate();

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
        pthread_create(&glc_thread, NULL, cpCaptureLoop, NULL);
}

static void * cpCaptureLoop(void * f)
{
        while (is_running) {
                
        }
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

static void cpException(char * err_msg)
{
        GtkWidget *d = gtk_message_dialog_new(GTK_WINDOW(window),
                                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                                     GTK_MESSAGE_ERROR,
                                                     GTK_BUTTONS_CLOSE,
                                                     err_msg);
        
        gtk_dialog_run(GTK_DIALOG(d));
        cpDestroy();
        exit(EXIT_FAILURE);
}
