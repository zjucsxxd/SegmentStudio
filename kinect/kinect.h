/* Grab kinect data */

#ifndef KINECT_H_
#define KINECT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libfreenect.h>
#include <pthread.h>

/* 
 *RAW RANGE [0-1024]
 *Convert raw bits to distance (cm).  Kinect data is inversly proportional 
 *MIN: .3003
 *Max: 5.34 (NOTE: CUT VALUES AFTER 1024, NOT VERY GOOD AFTER)
*/
#define KNTRAW2CM(x) 1.0f/(x * -0.0030711016 + 3.330945161)

/*Front (Double) buffers for public access */
uint16_t * knt_depth;

/* RGB friendly gray buffer, 8 BIT replicated in RGB */
char * knt_depth_rgb;

/* Initialize the kinect */
void kntInit();

/* Start capturing on kinet (ASYNC) */
void kntStart();

/* Stop capturing */
void kntStop();

/* Clean up */
void kntTerminate();

/* Thread which handles kinect stream data */
void * kntHandleDataThread(void * f);

/* Handle depth */
void kntDepthHandler();

#endif
