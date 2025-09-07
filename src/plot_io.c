#include <plot_io.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

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
