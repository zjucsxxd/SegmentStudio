/*
 * Copyright 2011 Inteligenesis - All Rights Reserved
 * 
 * Authors:
 *     Ellsworth Bud So Townsend [12/30/11]
 * 
 * Main application window handler
*/

#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <gtk/gtk.h>

/* Window properties */
#define MAIN_WINDOW_TITLE "Segment Studio" 
#define MAIN_WINDOW_SIZE 400  /* SIZExSIZE */

/* Window */
GtkWidget * _mw_main_window;

/* Start up main window */
void mwInit();

/* Main loop */
void mwLoop();

#endif
