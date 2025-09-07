cflags = -I ./include
SDL2_CFLAGS = $(shell pkg-config --cflags sdl2)
SDL2_LIBS   = $(shell pkg-config --libs sdl2) -lSDL2_gfx
CAIRO_CFLAGS = $(shell pkg-config --cflags cairo)
CAIRO_LIBS   = $(shell pkg-config --libs cairo)

#Release profile

obj = release/obj/main.o release/obj/plot_io.o release/obj/plot_graph.o \
	release/obj/plot_sdlwin.o

hdr = include/plot_io.h include/plot_graph.h include/plot_sdlwin.h

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

release/obj/plot_io.o: src/plot_io.c include/plot_io.h
	$(CC) src/plot_io.c -o release/obj/plot_io.o $(cflags)

release/obj/plot_graph.o: src/plot_graph.c include/plot_graph.h
	$(CC) src/plot_graph.c -o release/obj/plot_graph.o $(cflags)

release/obj/plot_sdlwin.o: src/plot_sdlwin.c include/plot_sdlwin.h
	$(CC) src/plot_sdlwin.c -o release/obj/plot_sdlwin.o $(cflags)

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

dbg = debug/obj/main.o debug/obj/plot_io.o debug/obj/plot_graph.o \
	debug/obj/plot_winsdl.h

debug_bin = debug/plot_devperf
CCGX = gcc -g -DDEBUG -o
CCG = gcc -g -DDEBUG -c
debug_command = $(CCGX) $(debug_bin) $(dbg) $(cflags) $(SDL2_CFLAGS) \
	$(SDL2_LIBS) $(CAIRO_CFLAGS) $(CAIRO_LIBS)

debug: $(dbg) $(hdr)
	$(debug_command)

debug/obj/main.o: src/main.c
	$(CCG) src/main.c -o debug/obj/main.o $(cflags)

debug/obj/plot_io.o: src/plot_io.c include/plot_io.h
	$(CCG) src/plot_io.c -o debug/obj/plot_io.o $(cflags)

debug/obj/plot_graph.o: src/plot_graph.c include/plot_graph.h
	$(CCG) src/plot_graph.c -o debug/obj/plot_graph.o $(cflags)

debug/obj/plot_sdlwin.o: src/plot_sdlwin.c include/plot_sdlwin.h
	$(CC) src/plot_sdlwin.c -o debug/obj/plot_sdlwin.o $(cflags)

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

