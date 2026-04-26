#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "waveform.h"

WaveformSample_t *data = NULL;

int load_data(void) {

    FILE *fp;
    size_t count = 0;
    char header[256];

    data = (WaveformSample_t *)malloc(SAMPLES * sizeof(WaveformSample_t));
    if (data == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Opens file in program root
    fp = fopen("power_quality_log.csv", "r");

    // Validate input
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Read and discard the header line
    if (fgets(header, sizeof(header), fp) == NULL) {
        printf("Error reading header.\n");
        fclose(fp);
        return 1;
    }

    // Read data from file
    while (fscanf(fp, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
        &data[count].time,
        &data[count].v_phA,
        &data[count].v_phB,
        &data[count].v_phC,
        &data[count].line_current,
        &data[count].frequency,
        &data[count].power_factor,
        &data[count].thd_percent) == 8) {
        if (++count > SAMPLES) {
            printf("Maximum number of samples (%d) reached.", SAMPLES);
            break;
        }
        }

    fclose(fp);

    printf("Number of lines read: %zu", count);

    return 0;
}

void free_data(void) {
    free(data);  // Free dynamically allocated memory
}

// Test function to verify output of function compute_p2p
int print_p2p(double *p2p_A, double *p2p_B, double *p2p_C) {
    printf("\nPeak-to-peak voltage for A: %lf", *p2p_A);
    printf("\nPeak-to-peak voltage for B: %lf", *p2p_B);
    printf("\nPeak-to-peak voltage for C: %lf", *p2p_C);
    return 0;
}