#include <cairo/cairo.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT 600
#define WINDOW_WIDTH 1000
#define MARGIN 80
#define PIXELS_PER_MONTH 100
#define MIN_WIDTH 1000

double devtime[1000];
double reviewtime[1000];
double leadtime[1000];
int num_months = 0;

void read_csv(const char *filename) {
    FILE *f = fopen(filename,"r");
    if(!f) {
		perror("Cannot open CSV");
		exit(1);
	}
    char line[1024];
    int line_num = 0;
    while(fgets(line, sizeof(line), f)) {
        char *token = strtok(line, ",");
        int i = 0;
        while(token) {
            double val = atof(token);
            if(line_num == 0)
				devtime[i] = val;
            else if(line_num == 1)
				reviewtime[i] = val;
            else if(line_num == 2)
				leadtime[i] = val;
            token = strtok(NULL, ",");
            i++;
        }
        if(i > num_months)
			num_months=i;
        line_num++;
        if(line_num > 2)
			break;
    }
    fclose(f);
}

void draw_plot(cairo_t *cr, int width) {
    double max_y=30.0;
    double y_scale=(HEIGHT - 2 * MARGIN) / max_y;
    double x_step = (num_months>1) ? (double)(width - 2 *
		MARGIN)/(num_months-1) : 10.0;
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);
    /* White background */
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    /* Axis */
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 2);
    cairo_move_to(cr, MARGIN, HEIGHT - MARGIN);
    cairo_line_to(cr, width - MARGIN, HEIGHT - MARGIN);
    cairo_stroke(cr);
    cairo_move_to(cr, MARGIN,HEIGHT - MARGIN);
    cairo_line_to(cr, MARGIN, MARGIN);
    cairo_stroke(cr);

    /* Labels and horizontal lines */
    cairo_set_font_size(cr, 12);
	int i = 0;
    for(; i <= 30; i++) {
        double y = HEIGHT-MARGIN - i * y_scale;
        char label[10];
        snprintf(label, sizeof label ,"%d", i);
        cairo_move_to(cr, MARGIN - 30, y + 5);
        cairo_show_text(cr, label);

        /* Horizontal line */
        cairo_save(cr);
        cairo_rectangle(cr, MARGIN, y , width - 2 * MARGIN, 1);
        cairo_clip(cr);
        cairo_set_line_width(cr, 1);
        cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
        cairo_move_to(cr, MARGIN, y);
        cairo_line_to(cr, width - MARGIN, y);
        cairo_stroke(cr);
        cairo_restore(cr);
    }

    /* Lables X: months since EPOCH (september 2025) */
    int month= 9, year = 2025;
    cairo_set_font_size(cr, 12);
	i = 0;
    for(; i < num_months; i++){
        double x= MARGIN + i * x_step;
        char label[30];
        snprintf(label, sizeof label, "%d/%d", month, year);
        cairo_move_to(cr,x - 20, HEIGHT - MARGIN + 20);
        cairo_show_text(cr,label);
        month++;
        if(month > 12) {
			month=1;
			year++;
		}
    }

    /* Draw series */
    void plot_series(double *data, double r, double g, double b) {
        cairo_set_source_rgb(cr, r, g, b);
        cairo_set_line_width(cr, 3);
        cairo_move_to(cr, MARGIN, HEIGHT - MARGIN - data[0] * y_scale);
		int i = 1;
        for(; i < num_months; i++){
            double x=MARGIN+i*x_step;
            double y=HEIGHT-MARGIN-data[i]*y_scale;
            cairo_line_to(cr,x,y);
        }
        cairo_stroke(cr);
    }
    plot_series(devtime, 1 ,0, 0);
    plot_series(reviewtime, 0, 0, 1);
    plot_series(leadtime,0 ,0.6, 0);

    /* Plot legend */
    double lx = width - MARGIN - 150, ly = 20;
    cairo_set_font_size(cr, 14);
    cairo_set_source_rgb(cr, 1, 0, 0);
	cairo_rectangle(cr, lx, ly, 20, 5);
	cairo_fill(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_move_to(cr, lx + 30, ly + 5);
	cairo_show_text(cr, "DevTime");
    ly+=25;
	cairo_set_source_rgb(cr, 0, 0, 1);
	cairo_rectangle(cr, lx, ly, 20, 5);
	cairo_fill(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_move_to(cr, lx + 30 , ly + 5);
	cairo_show_text(cr,"ReviewTime");
    ly += 25;
	cairo_set_source_rgb(cr, 0, 0.6, 0);
	cairo_rectangle(cr, lx, ly, 20, 5);
	cairo_fill(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_move_to(cr, lx + 30, ly + 5);
	cairo_show_text(cr, "LeadTime");
}

int main(void){
    read_csv("data.csv");
    int width = (num_months * PIXELS_PER_MONTH);
    if(width < MIN_WIDTH)
		width = MIN_WIDTH;
    unsigned char *data = malloc(width * HEIGHT * 4);
    cairo_surface_t *surface = cairo_image_surface_create_for_data(data,
		CAIRO_FORMAT_ARGB32, width, HEIGHT, width * 4);
    cairo_t *cr = cairo_create(surface);
    draw_plot(cr,width);
    cairo_surface_write_to_png(surface, "plot.png");
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
					last_mouse_x=e.motion.x;
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
        SDL_RenderCopy(renderer,texture,&src,&dst);
        SDL_RenderPresent(renderer);
    }
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    SDL_DestroyTexture(texture);
    free(data);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("Plot saved to plot.png and displayed in SDL window.\n");
    return 0;
}
