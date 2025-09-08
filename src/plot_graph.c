#include <plot_graph.h>
#include <limits.h>
#include <stdio.h>

#define LDEVTIME "DevTime"
#define LREVTIME "ReviewTime"
#define LLEADTIME "LeadTime"
#define MTATIME "MeanTAT"
#define LABELSP 18

/* Plot series */
static void plot_series(cairo_t *cr, int margin, int height, int nmonths,
	double y_scale, double x_step, double *data, double r, double g, double b);
/* Plot legend */
static void plot_legend(cairo_t *cr, int width, int margin);
/* Plot axis */
static void plot_axis(cairo_t *cr, int width, int height, int margin);
/* Plot lables and horizontal lines */
static void plot_labels(cairo_t *cr, int width, int height, int margin,
	int nmonths, double x_step, double y_scale, int st_month, int st_year);

void draw_plot(cairo_t *cr, int width, int height, int margin, int nmonths,
	double *dtime, double *rtime, double *ltime, double *mtime, int st_month,
    int st_year) {
    double max_y = 30.0;
    double y_scale = (height - 2 * margin) / max_y;
    double x_step = (nmonths > 1) ? (double) (width - 2 * margin) /
        (nmonths - 1) : 10.0;
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);
    /* White background */
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
	plot_axis(cr, width, height, margin);
	plot_labels(cr, width, height, margin, nmonths, x_step, y_scale, st_month,
        st_year);
    plot_series(cr, margin, height, nmonths, y_scale, x_step, dtime, 1, 0, 0);
    plot_series(cr, margin, height, nmonths, y_scale, x_step, rtime, 0, 0, 1);
    plot_series(cr, margin, height, nmonths, y_scale, x_step, ltime, 0, 0.6, 0);
    /* Compute mean turnarround time */
    int i = 0;
    for (; i < nmonths; i++)
        *(mtime + i) = (*(dtime + i) + *(rtime + i) + *(ltime + i)) / 3;
    plot_series(cr, margin, height, nmonths, y_scale, x_step, mtime, 1.0, 0.6, 0);
	plot_legend(cr, width, margin);
}


static void plot_series(cairo_t *cr, int margin, int height, int nmonths,
	double y_scale, double x_step, double *data, double r, double g, double b) {
	cairo_set_source_rgb(cr, r, g, b);
	cairo_set_line_width(cr, 3);
	cairo_move_to(cr, margin, height - margin - data[0] * y_scale);
	int i = 1;
	for (; i < nmonths; i++) {
		double x = margin + i * x_step;
		double y = height - margin - data[i] * y_scale;
		cairo_line_to(cr, x, y);
	}
	cairo_stroke(cr);
}

static void plot_legend(cairo_t *cr, int width, int margin)
{
    double lx = width - margin - 150, ly = LABELSP - 3;
    cairo_set_font_size(cr, 14);
    cairo_set_source_rgb(cr, 1, 0, 0);
	cairo_rectangle(cr, lx, ly, 20, 5);
	cairo_fill(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_move_to(cr, lx + 30, ly + 5);
	cairo_show_text(cr, LDEVTIME);
    ly += LABELSP;
	cairo_set_source_rgb(cr, 0, 0, 1);
	cairo_rectangle(cr, lx, ly, 20, 5);
	cairo_fill(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_move_to(cr, lx + 30 , ly + 5);
	cairo_show_text(cr, LREVTIME);
    ly += LABELSP;
	cairo_set_source_rgb(cr, 0, 0.6, 0);
	cairo_rectangle(cr, lx, ly, 20, 5);
	cairo_fill(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_move_to(cr, lx + 30, ly + 5);
	cairo_show_text(cr, LLEADTIME);
    ly += LABELSP;
	cairo_set_source_rgb(cr, 1, 0.6, 0);
	cairo_rectangle(cr, lx, ly, 20, 5);
	cairo_fill(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_move_to(cr, lx + 30, ly + 5);
	cairo_show_text(cr, MTATIME);
}

static void plot_axis(cairo_t *cr, int width, int height, int margin)
{
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 2);
    cairo_move_to(cr, margin, height - margin);
    cairo_line_to(cr, width - margin, height - margin);
    cairo_stroke(cr);
    cairo_move_to(cr, margin, height - margin);
    cairo_line_to(cr, margin, margin);
    cairo_stroke(cr);
}

static void plot_labels(cairo_t *cr, int width, int height, int margin,
	int nmonths, double x_step, double y_scale, int st_month, int st_year)
{
    cairo_set_font_size(cr, 12);
	int i = 0;
    for (; i <= 30; i++) {
        double y = height - margin - i * y_scale;
        char label[LINE_MAX];
        snprintf(label, sizeof label ,"%d", i);
        cairo_move_to(cr, margin - 30, y + 5);
        cairo_show_text(cr, label);
        /* Horizontal line */
        cairo_save(cr);
        cairo_rectangle(cr, margin, y , width - 2 * margin, 1);
        cairo_clip(cr);
        cairo_set_line_width(cr, 1);
        cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
        cairo_move_to(cr, margin, y);
        cairo_line_to(cr, width - margin, y);
        cairo_stroke(cr);
        cairo_restore(cr);
    }
    /* Lables X: months since EPOCH */
    cairo_set_font_size(cr, 12);
	i = 0;
    for (; i < nmonths; i++) {
        double x= margin + i * x_step;
        char label[LINE_MAX];
        snprintf(label, sizeof label, "%d/%d", st_month, st_year);
        cairo_move_to(cr,x - 20, height - margin + 20);
        cairo_show_text(cr,label);
        st_month++;
        if (st_month > 12) {
			st_month = 1;
			st_year++;
		}
    }
}
