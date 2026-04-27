#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "waveform.h"

WaveformSample_t *data = NULL;

int load_data(void) {
    size_t count = 0;
    char header[256];

    data = (WaveformSample_t *)malloc(SAMPLES * sizeof(WaveformSample_t));
    if (data == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Opens file in program root
    FILE *fp = fopen("power_quality_log.csv", "r");

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

    // TODO: handle cases where fscanf does not return 8
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

// TODO: Check pointer clearance is functional
void free_data(void) {
    free(data);  // Free dynamically allocated memory
    data = NULL;
}

void report_clipping() {
    int i = 0;

    for (i = 0; i < SAMPLES; i++) {
        if (data[i].health_flags & CLIP_A) {
            printf("\nClipping detected in phase A at %lf seconds.", data[i].time);
        }
        if (data[i].health_flags & CLIP_B) {
            printf("\nClipping detected in phase B at %lf seconds.", data[i].time);
        }
        if (data[i].health_flags & CLIP_C) {
            printf("\nClipping detected in phase C at %lf seconds.", data[i].time);
        }
    }
}

/* Test function to verify output of function compute_rms
int print_rms(double *rms_A, double *rms_B, double *rms_C) {
    printf("\nRMS for A: %lf", *rms_A);
    printf("\nRMS for B: %lf", *rms_B);
    printf("\nRMS for C: %lf", *rms_C);
} */

/* Test function to verify output of function compute_p2p
int print_p2p(double *p2p_A, double *p2p_B, double *p2p_C) {
    printf("\nPeak-to-peak voltage for A: %lf", *p2p_A);
    printf("\nPeak-to-peak voltage for B: %lf", *p2p_B);
    printf("\nPeak-to-peak voltage for C: %lf", *p2p_C);
    return 0;
} */

/* Test function to verify output of function compute_dc_offset
void print_dc_offset(double *mean_A, double *mean_B, double *mean_C) {
    printf("\nDC offset for A: %lf", *mean_A);
    printf("\nDC offset for B: %lf", *mean_B);
    printf("\nDC offset for C: %lf", *mean_C);
} */

/* Test function to verify output of function check_rms_tolerance
void print_tolerance_status(double* tolerance_status) {
    printf("\nTolerance status of A: %d", tolerance_status[0]);
    printf("\nTolerance status of B: %d", tolerance_status[1]);
    printf("\nTolerance status of C: %d", tolerance_status[2]);
} */