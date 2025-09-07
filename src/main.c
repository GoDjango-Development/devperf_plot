#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <plot_io.h>
#include <plot_graph.h>
#include <plot_sdlwin.h>

#define HEIGHT 600
#define WINDOW_WIDTH 1000
#define MARGIN 80
#define PIXELS_PER_MONTH 100
#define MIN_WIDTH 1000
#define MAX_MONTHS 1000
#define PLOTFILE "plot.png"
#define ECSV "Cannot open CSV"
#define ESURF "Cannot create drawing surface"
#define EWINDSDL "Cannot create SDL Window"
#define PLOTMSG "Plot saved to " PLOTFILE " and displayed in SDL window.\n"

double devtime[MAX_MONTHS];
double reviewtime[MAX_MONTHS];
double leadtime[MAX_MONTHS];
int num_months = 0;

int main(int argc, char **argv) {
    if (argc < 2)
        return EXIT_FAILURE;
    int st_month = 0;
    int st_year = 0;
    if (read_csv(*(argv + 1), devtime, reviewtime, leadtime, &num_months,
        &st_month, &st_year)) {
        perror(ECSV);
        return EXIT_FAILURE;
    }
    int width = (num_months * PIXELS_PER_MONTH);
    if(width < MIN_WIDTH)
		width = MIN_WIDTH;
    void *data = crtsurf_plot(PLOTFILE, width, HEIGHT, MARGIN, num_months,
        devtime, reviewtime, leadtime, st_month, st_year);
    if (!data) {
        perror(ESURF);
        return EXIT_FAILURE;
    }
    if (plot_crtwin(data, WINDOW_WIDTH, HEIGHT, width)) {
        perror(EWINDSDL);
        return EXIT_FAILURE;
    }
    printf(PLOTMSG);
    return EXIT_SUCCESS;;
}

