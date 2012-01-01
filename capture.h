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

#include <stdlib.h>
#include <pthread.h>
#include <gtk/gtk.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glfw.h>
#include "kinect/kinect.h"
#include "startup_mode.h"
#include "dseg.h"

/* Start capture utility */
void cpInit();

/* Close application */
static void cpDestroy();

/* Add capture button */
static void cpInitButton();

/* Handle capture button event */
static void cpCaptureButtonHandler(GtkWidget *widget, gpointer data);

/* Show folder dialog */
static void cpDirSelect();

/* Save the current image to a file */
static void cpSave();

/* 
 * NOTE: GL must be initialized and setup on the same thread 
 * Hence why there is a setup that is called as a thread
*/

/* Asynchronous call to GL setup */
static void cpInitGl();

/* Setup and cleanup opengl and capture */
static void * cpGlMain(void *f);

/* Handles capture and GL setup, called by main */
static void cpGlSetup();

/* Handles capture and GL rendering, called by main */
static void cpGlLoop();

/* Throw an exception */
static void cpException(char * err_msg);

#endif
