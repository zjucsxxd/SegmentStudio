#include "kinect.h"

/* Tells the main thread whether to keep running */
enum { STOPPED, RUNNING } knt_state;

/* Freenect context and device context */
freenect_context * knt_ctx;
freenect_device * knt_dev;

/* Thread which runs the data acqusition */
pthread_t knt_handle_data_thread;

/* Back (Double) buffers */
unsigned char * knt_back_depth;

void kntInit()
{
    /* Try to start freenect */
    if (freenect_init(&knt_ctx, NULL))
    {
        perror("Could not start freenect");
        exit(EXIT_FAILURE);
    }    

    /* Try to open the first Kinect */ 
    if (freenect_open_device(knt_ctx, &knt_dev, 0))
    {
        perror("Could not open kinect");
        exit(EXIT_FAILURE);
    }

    /* Set formatting */
    freenect_set_depth_format(knt_dev, FREENECT_DEPTH_11BIT); 

    /* Allocate buffers Depth=16bit rgba=24bit */
    knt_back_depth = malloc(640*480*2); /* Back buffer */
    knt_depth = malloc(640*480*2);      /* Front buffer */
    knt_depth_rgb = malloc(640*480*3);

    /* Load into back buffers */
    freenect_set_depth_buffer(knt_dev, knt_back_depth);

    /* Set callbacks */
    freenect_set_depth_callback(knt_dev, kntDepthHandler);
}

void kntStart()
{
    /* Enable streaming */
    freenect_start_depth(knt_dev);

    knt_state = RUNNING;

    //Launch thread
    pthread_create(&knt_handle_data_thread, NULL, kntHandleDataThread, NULL);
}

void kntStop()
{
    knt_state = STOPPED;
    pthread_join(knt_handle_data_thread, NULL);

    /* Stop streaming */
    freenect_stop_depth(knt_dev);
}

void kntTerminate()
{
    free(knt_back_depth);
    free(knt_depth);
    free(knt_depth_rgb);

    freenect_shutdown(knt_ctx);
}

void * kntHandleDataThread(void * f)
{
    while (knt_state == RUNNING)
        freenect_process_events(knt_ctx);
}

inline unsigned char kntCvtNormalizedByte(uint16_t in)
{
        return KNTRAW2CM(in)/4.0f*256.0f;
}

void kntDepthHandler()
{
    /* Swap buffer(s) front back to front */
    memcpy(knt_depth, knt_back_depth, 640*480*2);

    uint16_t * d_ptr = knt_depth;
    char * rgb_ptr = knt_depth_rgb;

    /* Clone depth for RGB friendly display */
    for (int i = 0; i < 640*480; ++i) {
        unsigned char d = kntCvtNormalizedByte(*d_ptr);
        *rgb_ptr = d;
        ++rgb_ptr;
        *rgb_ptr = d;
        ++rgb_ptr;
        *rgb_ptr = d;
        ++d_ptr;
        ++rgb_ptr;
    }
}
