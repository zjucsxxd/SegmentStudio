/*
 * Copyright 2011 Inteligenesis - All Rights Reserved
 * 
 * Authors:
 *     Ellsworth Bud So Townsend [12/30/11]
 * 
 * Linked list for pixel classifications
*/

#ifndef PIXEL_LIST_H_
#define PIXEL_LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include "dseg.h"

/* Linked list for dseg pixels */
typedef struct DSEGPIXELLIST
{
        DsegPixel pixel;
        struct DSEGPIXELLIST * next;
        
} DsegPixelList;

/* Setup a node */
void plInit(DsegPixelList * node, int x, int y, int label);

/* Delete a list */
void plDestroy(DsegPixelList * list);

/* Insert node into the list */
void plInsert(DsegPixelList * list, DsegPixelList * node);

/* Check if a pixel exists at position X Y */
int plDoesExist(DsegPixelList * list, int x, int y);

/* Count number of elements */
int plCount(DsegPixelList * list);

#endif
