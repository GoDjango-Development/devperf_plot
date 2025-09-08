#ifndef PLOT_IO_H
#define PLOT_IO_H

/* Create draw surface and output png plot */
void *crtsurf_plot(const char *plotfile, int width, int height, int margin,
    int nmonths, double *dtime, double *rtime, double *ltime, double *mtime,
    int st_month, int st_year);
/* Read CSV data file */
int read_csv(const char *file, double *dtime, double *rtime, double *ltime,
    int *nmonth, int *month, int *year);

#endif
