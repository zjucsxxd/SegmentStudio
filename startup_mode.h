/*
 * Copyright 2011 Inteligenesis - All Rights Reserved
 * 
 * Authors:
 *     Ellsworth Bud So Townsend [12/30/11]
 * 
 * When the program starts a dialog box pops up asking what mode the 
 * program should launch into.
*/

#ifndef STARTUP_MODE_H_
#define STARTUP_MODE_H_

#include <gtk/gtk.h>
#include "capture.h"
#include "viewer.h"

/* Start pop up dialog which will launch other parts of the program */
void smInit();

/* Destroy the window */
static void smDestroy();

/* Setup vertical box container */
static void smInitVbox();

/* Create buttons, called by init */
static void smInitButtons();

/* Information text */
static void smInitText();

/* Button callback handler */
static void smButtonCallback(GtkWidget *widget, gpointer data);

#endif
