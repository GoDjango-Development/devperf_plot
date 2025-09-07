#include <plot_sdlwin.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

/* SDL Window event loop */
static void event_loop(SDL_Renderer *renderer, int winwidth, int winheight,
	SDL_Texture *texture, int width);

int plot_crtwin(void *data, int winwidth, int winheight, int dym_width)
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "SDL_Initerror:%s\n", SDL_GetError());
		return 1;
	}
    SDL_Window *window = SDL_CreateWindow("Plot SDL", SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, winwidth, winheight, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED);
    SDL_Surface *sdl_surface = SDL_CreateRGBSurfaceFrom(data, dym_width,
		winheight, 32, dym_width * 4, 0x00ff0000, 0x0000ff00, 0x000000ff,
		0xff000000);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,sdl_surface);
    SDL_FreeSurface(sdl_surface);
    /* Window event loop */
    event_loop(renderer, winwidth, winheight, texture, dym_width);
    /* ----------------- */
    SDL_DestroyTexture(texture);
    free(data);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}

static void event_loop(SDL_Renderer *renderer, int winwidth, int winheight,
	SDL_Texture *texture, int dym_width)
{
    int offset_x = 0;
    int quit = 0;
    int dragging = 0, last_mouse_x = 0;
    SDL_Event e;
    do
        while (SDL_WaitEvent(&e)) {
            if (offset_x < 0)
                offset_x = 0;
            if (offset_x > dym_width - winwidth)
                offset_x = dym_width - winwidth;
            SDL_RenderClear(renderer);
            SDL_Rect src = { offset_x, 0, winwidth, winheight };
            SDL_Rect dst = { 0, 0, winwidth, winheight };
            SDL_RenderCopy(renderer, texture, &src, &dst);
            SDL_RenderPresent(renderer);
            if (e.type == SDL_QUIT) {
				quit = 1;
                break;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_LEFT)
					offset_x -= 20;
                if (e.key.keysym.sym == SDLK_RIGHT)
					offset_x += 20;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
					dragging = 1;
					last_mouse_x = e.button.x;
				}
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                if (e.button.button == SDL_BUTTON_LEFT)
					dragging = 0;
			} else if (e.type == SDL_MOUSEMOTION) {
                if (dragging) {
					offset_x += last_mouse_x - e.motion.x;
					last_mouse_x = e.motion.x;
				}
            }
        }
    while (!quit);
}
