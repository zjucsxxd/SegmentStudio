main:
	gcc -std=c99 main.c startup_mode.c capture.c -o main `pkg-config libglfw --libs --cflags` `pkg-config glu --libs --cflags` `pkg-config gtk+-2.0 --libs --cflags`
