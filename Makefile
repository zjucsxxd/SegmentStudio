main:
	gcc -std=c99 main.c startup_mode.c capture.c -o main `pkg-config --cflags --libs gtk+-2.0`
