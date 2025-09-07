#include <cairo/cairo.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <plot_io.h>
#include <plot_graph.h>

#define HEIGHT 600
#define WINDOW_WIDTH 1000
#define MARGIN 80
#define PIXELS_PER_MONTH 100
#define MIN_WIDTH 1000
#define MAX_MONTHS 1000
#define PLOTFILE "plot.png"
#define ECSV "Cannot open CSV"
#define ESURF "Cannot create drawing surface"

double devtime[MAX_MONTHS];
double reviewtime[MAX_MONTHS];
double leadtime[MAX_MONTHS];
int num_months = 0;

/* Create draw surface and output png plot */
static void *crtsurf_plot(int width);

int main(int argc, char **argv) {
    if (argc < 2)
        return EXIT_FAILURE;
    if (read_csv(*(argv + 1), devtime, reviewtime, leadtime, &num_months)) {
        perror(ECSV);
        return EXIT_FAILURE;
    }
    int width = (num_months * PIXELS_PER_MONTH);
    if(width < MIN_WIDTH)
		width = MIN_WIDTH;
    unsigned char *data = crtsurf_plot(width);
    if (!data) {
        perror(ESURF);
        return EXIT_FAILURE;
    }
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "SDL_Initerror:%s\n", SDL_GetError());
		return 1;
	}
    SDL_Window *window = SDL_CreateWindow("Plot SDL", SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED);
    SDL_Surface *sdl_surface = SDL_CreateRGBSurfaceFrom(data, width, HEIGHT,
		32, width * 4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,sdl_surface);
    SDL_FreeSurface(sdl_surface);
    int offset_x = 0;
    int quit = 0;
    int dragging = 0, last_mouse_x = 0;
    SDL_Event e;
    while(!quit){
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT)
				quit = 1;
            else if(e.type == SDL_KEYDOWN) {
                if(e.key.keysym.sym == SDLK_LEFT)
					offset_x -= 20;
                if(e.key.keysym.sym == SDLK_RIGHT)
					offset_x += 20;
            } else if(e.type == SDL_MOUSEBUTTONDOWN){
                if(e.button.button == SDL_BUTTON_LEFT) {
					dragging = 1;
					last_mouse_x = e.button.x;
				}
            } else if(e.type == SDL_MOUSEBUTTONUP){
                if(e.button.button == SDL_BUTTON_LEFT)
					dragging = 0;
			} else if(e.type == SDL_MOUSEMOTION){
                if(dragging) {
					offset_x += last_mouse_x - e.motion.x;
					last_mouse_x = e.motion.x;
				}
            }
        }
        if(offset_x < 0)
			offset_x = 0;
        if(offset_x > width - WINDOW_WIDTH)
			offset_x = width-WINDOW_WIDTH;
        SDL_RenderClear(renderer);
        SDL_Rect src = { offset_x, 0, WINDOW_WIDTH, HEIGHT };
        SDL_Rect dst = { 0, 0, WINDOW_WIDTH, HEIGHT };
        SDL_RenderCopy(renderer,texture, &src, &dst);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    free(data);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("Plot saved to plot.png and displayed in SDL window.\n");
    return EXIT_SUCCESS;;
}

static void *crtsurf_plot(int width)
{
    unsigned char *data = malloc(width * HEIGHT * 4);
    if (!data)
        return NULL;
    cairo_surface_t *surface = cairo_image_surface_create_for_data(data,
		CAIRO_FORMAT_ARGB32, width, HEIGHT, width * 4);
    cairo_t *cr = cairo_create(surface);
    draw_plot(cr, width, HEIGHT, MARGIN, num_months, devtime, reviewtime,
        leadtime);
    cairo_surface_write_to_png(surface, PLOTFILE);
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    return data;
}
