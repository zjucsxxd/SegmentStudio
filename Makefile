main:
	gcc main.c main_window.c startup_mode.c -o main `pkg-config --cflags --libs gtk+-2.0`
