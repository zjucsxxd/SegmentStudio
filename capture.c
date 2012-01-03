#include "capture.h"

/* Window properties */
#define WINDOW_TITLE "Save capture"
#define WINDOW_SIZE 80  /* SIZExSIZE */

/* GL viewport context properties */
#define GL_TITLE "Say cheese!"
#define GL_WIDTH 800
#define GL_HEIGHT 600

/* Window */
static GtkWidget * window;
static GtkWidget * capture_button;

/* Folder path */
static gchar * path;

/* GL thread */
static int gl_is_running = TRUE;
static pthread_t gl_thread;

void cpInit()
{
        gtk_init(NULL, NULL);
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

        gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
        gtk_container_set_border_width(GTK_CONTAINER(window), WINDOW_SIZE);
        gtk_window_set_policy(GTK_WINDOW(window), FALSE, FALSE, FALSE);

        /* Get directory */
        cpDirSelect();
        
        cpInitButton();
        cpInitGl();

        gtk_widget_show_all(window);

        gtk_main();
}

static void cpDestroy()
{
        /* Kill GL */
        gl_is_running = FALSE;
        pthread_join(gl_thread, NULL); 

        gtk_widget_destroy(window);
        gtk_main_quit();
}

static void cpInitButton()
{
        capture_button = gtk_button_new_with_label("Capture!");
        gtk_container_add(GTK_CONTAINER(window), capture_button);

        g_signal_connect(capture_button, "clicked", G_CALLBACK(cpCaptureButtonHandler), NULL);
}

static void cpCaptureButtonHandler(GtkWidget *widget, gpointer data)
{
        cpSave();
}

static void cpInitGl()
{
        pthread_create(&gl_thread, NULL, cpGlMain, NULL);
}

static void * cpGlMain(void *f)
{
        if (!glfwInit())
                cpException("Could not start openGL");

        glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);

        if (!glfwOpenWindow(GL_WIDTH,
                       GL_HEIGHT,
                       8, /* 24-bit RGB */
                       8,
                       8,
                       0,
                       0, /* Depth bits */
                       0, /* Stencil bits */
                       GLFW_WINDOW)) {
                cpException("Could not open an openGL window");        
        }

        glfwSetWindowTitle(GL_TITLE);
        glfwPollEvents();

        /* Start kinect */
        kntInit();
        kntStart();

        cpGlSetup();
        cpGlLoop();

        /* Clean up kinect */
        kntTerminate();

        glfwTerminate();
}

static void cpGlSetup()
{
        
        /* Flags */
        glEnable(GL_TEXTURE_2D);
        
        /* Texture settings */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        /* Perspective */
        glMatrixMode(GL_PROJECTION);
        gluPerspective(45.0f, 640.0f/480.0f, 0.0f, 100.0f);
        glMatrixMode(GL_MODELVIEW);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, 0.0f, -3.0f);
}

static void cpGlLoop()
{
        while (gl_is_running) {
                glClear(GL_COLOR_BUFFER_BIT);
        
                /* Texture image */
                glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, knt_depth_rgb);
                glBegin(GL_QUADS);
                        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.33f, 1.0f);
                        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.33f, 1.0f);
                        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.33f, -1.0f);
                        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.33f, -1.0f);
                glEnd();

                glfwSwapBuffers();
                glfwSleep(.01);
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

static void cpSave()
{
        /* Name files 0.dseg 1.dseg...etc */
        static int count = -1;
        ++count;

        char file_path[4096];
        sprintf(file_path, "%s/%d.dseg", path, count);

        /* Remove the file:// from the front of the path */
        memcpy(file_path, &file_path[strlen("file://")], 4096-strlen("file://"));

        FILE * f = fopen(file_path, "w");

        if (!f) {
                perror("Couldn't write to directory.  Do you have write permissions?");
                exit(EXIT_FAILURE);
        }

        /* Create header */
        DsegHeader dh;
        dh.num_pixels_classified = 0;
        memcpy(dh.dbuf, knt_depth, sizeof(dh.dbuf));

        fwrite(&dh, 1, sizeof(dh), f);

        fclose(f);
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
