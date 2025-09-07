cflags = -Wall -O2 -I ./include
SDL2_CFLAGS = $(shell pkg-config --cflags sdl2)
SDL2_LIBS   = $(shell pkg-config --libs sdl2) -lSDL2_gfx
CAIRO_CFLAGS = $(shell pkg-config --cflags cairo)
CAIRO_LIBS   = $(shell pkg-config --libs cairo)

#Release profile

obj = release/obj/main.o

hdr =

plot_file = config/data.csv

release_bin = release/plot_devperf
CCX = gcc -o
CC = gcc -c
release_command = $(CCX) $(release_bin) $(obj) $(cflags) $(SDL2_CFLAGS) \
	$(SDL2_LIBS) $(CAIRO_CFLAGS) $(CAIRO_LIBS)

release: $(obj) $(hdr)
	$(release_command)

release/obj/main.o: src/main.c
	$(CC) src/main.c -o release/obj/main.o $(cflags)


run: release
ifneq ("$(wildcard $(release_bin))","")
	$(release_bin) $(plot_file)
else
	$(release_command)
	$(release_bin) $(plot_file)
endif

.PHONY: rebuild

rebuild:
	$(MAKE) clean
	$(MAKE) release

# Debug profile

dbg = debug/obj/main.o

debug_bin = debug/plot_devperf
CCGX = gcc -g -DDEBUG -o
CCG = gcc -g -DDEBUG -c
debug_command = $(CCGX) $(debug_bin) $(dbg) $(cflags) $(SDL2_CFLAGS) \
	$(SDL2_LIBS) $(CAIRO_CFLAGS) $(CAIRO_LIBS)

debug: $(dbg) $(hdr)
	$(debug_command)

debug/obj/main.o: src/main.c
	$(CCG) src/main.c -o debug/obj/main.o $(cflags)

run_debug: debug
ifneq ("$(wildcard $(debug_bin))","")
	gdb $(debug_bin)
else
	$(debug_command)
	gdb $(debug_bin)
endif

.PHONY: rebuild_debug

rebuild_debug:
	$(MAKE) clean
	$(MAKE) debug

#Others rules

.PHONY: prepare clean

prepare:
	mkdir -p debug release debug/obj release/obj
	mkdir -p src include conf
clean:
	rm -R -f $(obj) $(dbg) release/obj/* debug/obj/*
	rm -R -f $(release_bin) $(debug_bin)

