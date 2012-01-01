/*
 * Copyright 2011 Inteligenesis - All Rights Reserved
 * 
 * Authors:
 *     Ellsworth Bud So Townsend [12/30/11]
 * 
 * Allows a frame to be captured
*/

#ifndef CAPTURE_H_
#define CAPTURE_H_

#include <gtk/gtk.h>
#include "startup_mode.h"

/* Start capture utility */
void cpInit();

/* Close application */
static void cpDestroy();

/* Add vbox container */
static void cpInitVbox();

/* Add and start capture feed */
static void cpInitCapture();

/* Show folder dialog */
static void cpDirSelect();

#endif
