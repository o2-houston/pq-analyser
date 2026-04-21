/* All file I/O: loading the CSV into your struct array, writing the results file. */

#include <stdio.h>

#define SAMPLES 1000

typedef struct {
    double time;
    double v_phA;
    double v_phB;
    double v_phC;
    double line_current;
    double frequency;
    double power_factor;
    double thd_percent;
} data_t;

data_t data[SAMPLES];

int main(void) {

    FILE *fp;
    size_t count = 0;

    // Opens file in program root
    fp = fopen("power_quality_log.csv", "r");

    // Validate input
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Currently only handles numeric-only CSV (no heading support yet)
    // To be resolved with the original, unedited file correctly being handled as program input
    while (fscanf(fp, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
        &data[count].time,
        &data[count].v_phA,
        &data[count].v_phB,
        &data[count].v_phC,
        &data[count].line_current,
        &data[count].frequency,
        &data[count].power_factor,
        &data[count].thd_percent) == 8) {
        count ++;
        }

    // Print values to screen to verify data has been read successfully
    for (size_t i = 0; i < count; i++) {
        printf("%f, %f, %f, %f, %f, %f, %f, %f\n",
            data[i].time,
            data[i].v_phA,
            data[i].v_phB,
            data[i].v_phC,
            data[i].line_current,
            data[i].frequency,
            data[i].power_factor,
            data[i].thd_percent);
    }

    fclose(fp);

    printf("Number of lines read: %zu", count);

    return 0; // Success
}