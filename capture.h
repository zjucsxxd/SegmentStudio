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
#include "startup_mode.h"

/* Start capture utility */
void cpInit();

/* Load OpenGL context (GLFW) */
static void cpInitGLWindow();

/* Close application */
static void cpDestroy();

/* Add vbox container */
static void cpInitVbox();

/* Add and start capture feed */
static void cpInitCapture();

/* Thread for capture GL window */
static void * cpCaptureLoop(void * f);

/* Show folder dialog */
static void cpDirSelect();

/* Throw an exception */
static void cpException(char * err_msg);

#endif
