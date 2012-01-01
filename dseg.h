/*
 * Copyright 2011 Inteligenesis - All Rights Reserved
 * 
 * Authors:
 *     Ellsworth Bud So Townsend [12/30/11]
 * 
 * DepthSEGment file format [.dseg]
*/

#ifndef DSEG_H_
#define DSEG_H_

/* Header for a dseg file */
typedef struct
{
        /* Depth buffer */
        uint16_t dbuf[640*480];
        
        /* How many pixels are classified */
        int num_pixels_classified;
} DsegHeader;

/* Following the header is an array of classifications */
typedef struct
{
        /* Location in the depth buffer this pixel points to */
        char x, y;
        
        /* Label that applies to this pixel */
        char label; 
} DsegPixel;

#endif
