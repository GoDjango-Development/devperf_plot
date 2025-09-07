#include <plot_io.h>
#include <plot_graph.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <cairo/cairo.h>

#define DEVTIME 0
#define REVTIME 1
#define LEADTIME 2

int read_csv(const char *file, double *dtime, double *rtime, double *ltime,
    int *nmonth) {
    FILE *f = fopen(file, "r");
    if (!f)
		return -1;
    char line[LINE_MAX];
    int line_num = 0;
    while(fgets(line, sizeof(line), f)) {
        char *tok = strtok(line, ",");
        int i = 0;
        while(tok) {
            double val = atof(tok);
            if(line_num == DEVTIME)
				*(dtime + i) = val;
            else if(line_num == REVTIME)
				*(rtime + i) = val;
            else if(line_num == LEADTIME)
				*(ltime + i) = val;
            tok = strtok(NULL, ",");
            i++;
        }
        if(i > *nmonth)
			*nmonth = i;
        line_num++;
        if(line_num > 2)
			break;
    }
    fclose(f);
	return 0;
}

void *crtsurf_plot(const char *plotfile, int width, int height, int margin,
    int nmonths, double *dtime, double *rtime, double *ltime)
{
    unsigned char *data = malloc(width * height * 4);
    if (!data)
        return NULL;
    cairo_surface_t *surface = cairo_image_surface_create_for_data(data,
		CAIRO_FORMAT_ARGB32, width, height, width * 4);
    cairo_t *cr = cairo_create(surface);
    draw_plot(cr, width, height, margin, nmonths, dtime, rtime, ltime);
    cairo_surface_write_to_png(surface, plotfile);
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    return data;
}
