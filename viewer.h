/*
 * Copyright 2011 Inteligenesis - All Rights Reserved
 * 
 * Authors:
 *     Ellsworth Bud So Townsend [12/30/11]
 * 
 * View and edit dseg files
*/

#ifndef VIEWER_H_
#define VIEWER_H_

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glfw.h>
#include <pthread.h>
#include "kinect/kinect.h"
#include "dseg.h"
#include "pixel_list.h"

/* Start viewer */
void viInit();

/* Create vbox */
static void viInitVbox();

/* Shut the program down */
static void viDestroy();

/* Create file menu */
static void viInitFileButton();

/* Handle file button press */
static void viFileButtonHandler(GtkWidget *widget, gpointer data);

/* Update file button text */
static void viUpdateFileButton();

/* Let user pick a file and open the file */
static void viDialogOpenFile();

/* Load the file that is the current context */
static void viLoadFile();

/* Save the file */
static void viSaveFile();

/* Create toolbar */
static void viInitToolbar();

/* Handle toolbar clicks */
static void viToolbarHandler(GtkWidget *widget, gpointer data);

/* 
 * NOTE: GL must be initialized and setup on the same thread 
 * Hence why there is a setup that is called as a thread
*/

/* Setup and cleanup opengl */
static void * viGlMain(void *f);

/* Handles GL setup, called by main */
static void viGlSetup();

/* Handles GL rendering, called by main */
static void viGlLoop();

/* Close down GL */
static void viGlShutdown();

/* Draw pixel to backbuffer */
static void viDrawPixel(DsegPixelList * node);

/* 
 * Add to pixel list 
 * if label > 0, the label value is used instead of global radio
*/
static void viAddPixel(int x, int y, int label);

/* Add a square of pixels to the pixel list */
static void viAddSquare(int x, int y, int radius);

/* Drawing mouse click handler */
static void viDrawingMouseClickHandler(int button, int action);

/* Drawing mouse movement handler */
static void viDrawingMouseMoveHandler(int x, int y);

/* Throw an exception */
static void viException(char * err_msg);

#endif
