main:
	gcc -O3 -std=c99 pixel_list.c main.c startup_mode.c capture.c viewer.c kinect/kinect.c -o main `pkg-config libglfw --libs --cflags` `pkg-config glu --libs --cflags` `pkg-config gtk+-2.0 --libs --cflags` -lfreenect -lglfw
