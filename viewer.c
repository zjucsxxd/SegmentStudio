#include "viewer.h"

/* Window properties */
#define WINDOW_TITLE "Viewer / Editor"
#define WINDOW_SIZE 100 /* SIZExSIZE */

/* GL viewport properties */
#define GL_WIDTH 640
#define GL_HEIGHT 480
#define GL_TITLE "Image"

/* GL thread */
static int gl_is_running = TRUE;
static pthread_t gl_thread;

/* Window */
static GtkWidget * window = NULL;
static GtkWidget * vbox = NULL;

/* Type toolbar */
static GtkWidget * toolbar_vbox = NULL;
static GtkWidget *toolbar[DSEG_MAX_TYPES];

/* Open file */
static GtkWidget * file_vbox = NULL;
static GtkWidget * file_button = NULL;
static enum { OPEN, SAVE_AND_CLOSE } file_button_state;
DsegHeader open_file_header;
char * filename;

/* Drawing states */
#define DRAW_RADIUS 15
static enum { NO_DRAW, DRAW } draw_state;
static int current_label = 0;
char depth_rgb_surface[640*480*3];
DsegPixelList * pixel_list = NULL;
DsegPixelList * pixel_list_end = NULL;

void viInit()
{
        gtk_init(NULL, NULL);
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

        gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
        gtk_container_set_border_width(GTK_CONTAINER(window), WINDOW_SIZE);
        gtk_window_set_policy(GTK_WINDOW(window), FALSE, FALSE, FALSE);

        viInitVbox();
        viInitFileButton();
        viInitToolbar();

        gtk_widget_show_all(window);
        gtk_main();
}

static void viInitVbox()
{
       vbox = gtk_vbox_new(TRUE, 0);
       gtk_container_add(GTK_CONTAINER(window), vbox); 
}

static void viInitFileButton()
{
        /* Create new vbox */
        file_vbox = gtk_vbox_new(TRUE, 0);
        gtk_container_add(GTK_CONTAINER(vbox), file_vbox);

        /* Open file button */
        file_button = gtk_button_new();
        viUpdateFileButton();
        gtk_container_add(GTK_CONTAINER(file_vbox), file_button);
        g_signal_connect(file_button, "clicked", G_CALLBACK(viFileButtonHandler), NULL);
}

static void viFileButtonHandler(GtkWidget *widget, gpointer data)
{
        switch (file_button_state) {
        case OPEN:
                viDialogOpenFile();
                break;
        case SAVE_AND_CLOSE:
                viSaveFile();
                file_button_state = OPEN;
                viUpdateFileButton();
                break;
        }
}

static void viDialogOpenFile()
{
        GtkWidget * fd = gtk_file_chooser_dialog_new("Please select a dseg file",
                                                     GTK_WINDOW(window),
                                                     GTK_FILE_CHOOSER_ACTION_OPEN,
                                                     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                     GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                                     NULL);
        
        if (gtk_dialog_run(GTK_DIALOG(fd)) == GTK_RESPONSE_ACCEPT) {
                file_button_state = SAVE_AND_CLOSE;
                viUpdateFileButton();
                filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fd));
                viLoadFile();
        }

        gtk_widget_destroy(fd);
}

static void viLoadFile()
{
        FILE * f = fopen(filename, "r");
        
        if (!f)
                viException("Could not open file for reading");

        int nread = fread(&open_file_header, 1, sizeof(open_file_header), f);

        if (nread != sizeof(open_file_header))
                viException("Corrupt dseg file.  Not big enough");

         /* Convert depth to RGB image */
        char * rgb_ptr = depth_rgb_surface;
        uint16_t * raw_ptr = open_file_header.dbuf;

        for (int i = 0; i < 640*480; ++i) {
                unsigned char norm = kntCvtNormalizedByte(*raw_ptr);
                for (int i = 0; i < 3; ++i)  {
                        *rgb_ptr = norm;
                        ++rgb_ptr;
                }

                ++raw_ptr;
        }


        for (int i = 0; i < open_file_header.num_pixels_classified; ++i) {
                DsegPixel pixel;
                fread(&pixel, 1, sizeof(pixel), f);
                viAddPixel(pixel.x, pixel.y, pixel.label);
        }

        pthread_create(&gl_thread, NULL, viGlMain, NULL);
}

static void viSaveFile()
{
        FILE * f = fopen(filename, "w");
        
        if (!f)
                viException("Could not open file for writing");

        /* Update header */
        open_file_header.num_pixels_classified = plCount(pixel_list); 

        /* Write header back */
        fwrite(&open_file_header, 1, sizeof(open_file_header), f);

        /* Write pixels */
        while (pixel_list) {
                fwrite(&pixel_list->pixel, 1, sizeof(pixel_list->pixel), f);
                pixel_list = pixel_list->next; 
        }

        fclose(f);

        viGlShutdown();
}

static void viUpdateFileButton()
{
        switch (file_button_state) {
        case OPEN:
                gtk_button_set_label(GTK_BUTTON(file_button), "Open Segment File");
                break;
        case SAVE_AND_CLOSE:
                gtk_button_set_label(GTK_BUTTON(file_button), "Save & Close");
                break;
        }
}

static void viInitToolbar()
{
        /* Create new vbox */
        toolbar_vbox = gtk_vbox_new(TRUE, 0);
        gtk_container_add(GTK_CONTAINER(vbox), toolbar_vbox); 

        /* Label */
        GtkWidget *label = gtk_label_new("Classification Types");
        gtk_container_add(GTK_CONTAINER(toolbar_vbox), label);

        /* Add first button */
        toolbar[0] = gtk_radio_button_new_with_label(NULL, "Class A");

        /* Build other buttons */
        for (int i = 1; i < DSEG_MAX_TYPES; ++i) {
                /* Create class label */
                static int letter = 'A';
                ++letter;

                char label[strlen("Class A")];
                sprintf(label, "Class %c", letter);

                /* Get group */
                GSList * group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(toolbar[i-1]));

                toolbar[i] = gtk_radio_button_new_with_label(group, label);
        }

        for (int i = 0; i < DSEG_MAX_TYPES; ++i) {
                gtk_container_add(GTK_CONTAINER(toolbar_vbox), toolbar[i]);
                g_signal_connect(toolbar[i], "clicked", G_CALLBACK(viToolbarHandler), NULL);
        }

}

static void viToolbarHandler(GtkWidget *widget, gpointer data)
{
        /* Find the segment group this button belongs to */
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
                /* Get class of widget */
                for (int i = 0; i < DSEG_MAX_TYPES; ++i) {
                        if (widget == toolbar[i]) {
                                current_label = i;
                                break;
                        }
                }
        }
}

static void viUpdatePixels()
{
        DsegPixelList * list = pixel_list;

        while (list) {
                
                list = list->next;
        }
}

static void viDestroy()
{
        viGlShutdown();

        gtk_widget_destroy(window);
        gtk_main_quit();
}

static void * viGlMain(void *f)
{
        if (!glfwInit())
                viException("Could not start openGL");

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
                viException("Could not open an openGL window");        
        }

        glfwSetWindowTitle(GL_TITLE);
        glfwPollEvents();

        glfwSetMouseButtonCallback(viDrawingMouseClickHandler);
        glfwSetMousePosCallback(viDrawingMouseMoveHandler);

        viGlSetup();
        viGlLoop();

        glfwTerminate();
}

static void viGlSetup()
{
        /* Flags */
        glEnable(GL_TEXTURE_2D);
        
        /* Texture settings */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

static void viGlLoop()
{
        gl_is_running = 1;

        while (gl_is_running) {
                glClear(GL_COLOR_BUFFER_BIT);

                /* Load file texture */
                glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, depth_rgb_surface);
                glBegin(GL_QUADS);
                        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
                        glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
                        glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
                        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
                glEnd();

                glfwSwapBuffers();
                glfwSleep(.01);
        }
}

static void viGlShutdown()
{
        if (!gl_is_running)
                return;

        gl_is_running = 0;
        pthread_join(gl_thread, NULL);

        plDestroy(pixel_list);
        
        pixel_list = NULL;
        pixel_list_end = NULL;
        draw_state = NO_DRAW;
}

/* For unpacking the texture to draw on */
typedef struct
{
        char r, g, b;
} Crgb;

static void viDrawPixel(DsegPixelList * node)
{
        /* Get label */
        int label = node->pixel.label;
        int r = (label == 0) * 0xFF + (label == 3) * 0xFF;
        int g = (label == 1) * 0xFF + (label == 3) * 0xFF + (label == 4) * 0xFF;
        int b = (label == 2) * 0xFF + (label == 4) * 0xFF;

        Crgb * ptr = (Crgb *)&depth_rgb_surface;

        int x = node->pixel.x;
        int y = node->pixel.y;

        ptr[y*640+x].r = r;
        ptr[y*640+x].g = g;
        ptr[y*640+x].b = b;
}

static void viDrawingMouseClickHandler(int button, int action)
{
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
                if (action == GLFW_PRESS) {
                        draw_state = DRAW;
                } else {
                        draw_state = NO_DRAW;
                }
        }
}

static void viDrawingMouseMoveHandler(int x, int y)
{
        if (draw_state == DRAW)
                viAddSquare(x, y, DRAW_RADIUS);
}

static void viAddPixel(int x, int y, int label)
{
        if (x < 0 || x > 640 || y < 0 || y > 480)
                return;

        
        if (!plDoesExist(pixel_list, x, y)) {
                DsegPixelList * node = malloc(sizeof(*node));

                if (label > -1)
                        plInit(node, x, y, label); /* loading file */
                else
                        plInit(node, x, y, current_label);

                if (!pixel_list)
                        pixel_list = node;
                
                if (pixel_list_end)
                        pixel_list_end->next = node;

                pixel_list_end = node;

                viDrawPixel(node);
        }
}

static void viAddSquare(int x, int y, int radius)
{
        for (int u = -radius; u < radius; ++u)
                for (int v = -radius; v < radius; ++v)
                        viAddPixel(x+u, y+v, -1);
}

static void viException(char * err_msg)
{
        GtkWidget *d = gtk_message_dialog_new(GTK_WINDOW(window),
                                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                                     GTK_MESSAGE_ERROR,
                                                     GTK_BUTTONS_CLOSE,
                                                     err_msg);
        
        gtk_dialog_run(GTK_DIALOG(d));
        viDestroy();
        exit(EXIT_FAILURE);
}
