#ifndef PLOT_GRAPH_H
#define PLOT_GRAPH_H

#include <cairo/cairo.h>

void draw_plot(cairo_t *cr, int width, int height, int maring, int nmonths,
	double *dtime, double *rtime, double *ltime);

#endif
