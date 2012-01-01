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
#include "main_window.h"

/* Start pop up dialog which will launch other parts of the program */
void smInit();

/* Setup vertical box container */
void smInitVbox();

/* Create buttons, called by init */
static void smInitButtons();

/* Information text */
static void smInitText();

/* Main loop */
static void smLoop();

#endif
