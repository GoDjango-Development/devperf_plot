CC = gcc
CFLAGS = -Wall -O2
SDL2_CFLAGS = $(shell pkg-config --cflags sdl2)
SDL2_LIBS   = $(shell pkg-config --libs sdl2) -lSDL2_gfx
CAIRO_CFLAGS = $(shell pkg-config --cflags cairo)
CAIRO_LIBS   = $(shell pkg-config --libs cairo)
TARGET = plot
SRC = plot.c

all: release

release:
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) $(CAIRO_CFLAGS) $(SRC) -o $(TARGET) \
	$(SDL2_LIBS) $(CAIRO_LIBS)

clean:
	rm -f $(TARGET) *.o plot.png
