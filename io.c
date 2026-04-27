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
        printf("Error opening input file.\n");
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

// TODO: add total samples read, date/time of export
// TODO: restructure function outputs into struct for cleaner function definition
int file_output(double *rms_A, double *rms_B, double *rms_C, double *p2p_A, double *p2p_B, double *p2p_C, double *mean_A, double *mean_B, double *mean_C, int* tolerance_status, double *var_A, double *var_B, double *var_C, double *stddev_A, double *stddev_B, double *stddev_C) {

    // Validate file output
    FILE *fp = fopen("output.txt", "w");
    if (fp == NULL) {
        printf("Error opening output file.");
        return 1;
    }

    fprintf(fp, "====== RMS VALUES ======\n");
    fprintf(fp, "\nRMS for A: %lfV", *rms_A);
    fprintf(fp, "\nRMS for B: %lfV", *rms_B);
    fprintf(fp, "\nRMS for C: %lfV\n", *rms_C);

    fprintf(fp, "\n====== PEAK-TO-PEAK VALUES ======\n");
    fprintf(fp, "\nPeak-to-peak voltage for A: %lfV", *p2p_A);
    fprintf(fp, "\nPeak-to-peak voltage for B: %lfV", *p2p_B);
    fprintf(fp, "\nPeak-to-peak voltage for C: %lfV\n", *p2p_C);

    fprintf(fp, "\n====== DC OFFSET VALUES ======\n");
    fprintf(fp, "\nDC offset for A: %lfV", *mean_A);
    fprintf(fp, "\nDC offset for B: %lfV", *mean_B);
    fprintf(fp, "\nDC offset for C: %lfV\n", *mean_C);

    fprintf(fp, "\n====== VOLTAGE TOLERANCE STATUS ======\n");

    // Phase A tolerance
    fprintf(fp, "\nPhase A: ");
    switch (tolerance_status[0]) {
        case 1: fprintf(fp, "Above tolerance threshold."); break;
        case 0: fprintf(fp, "Within tolerance threshold."); break;
        case -1: fprintf(fp, "Below tolerance threshold."); break;
        default : fprintf(fp, "Unable to read tolerance status.");
    }

    // Phase B tolerance
    fprintf(fp, "\nPhase B: ");
    switch (tolerance_status[1]) {
        case 1: fprintf(fp, "Above tolerance threshold."); break;
        case 0: fprintf(fp, "Within tolerance threshold."); break;
        case -1: fprintf(fp, "Below tolerance threshold."); break;
        default : fprintf(fp, "Unable to read tolerance status.");
    }

    // Phase C tolerance
    fprintf(fp, "\nPhase C: ");
    switch (tolerance_status[2]) {
        case 1: fprintf(fp, "Above tolerance threshold."); break;
        case 0: fprintf(fp, "Within tolerance threshold."); break;
        case -1: fprintf(fp, "Below tolerance threshold."); break;
        default : fprintf(fp, "Unable to read tolerance status.");
    }

    fprintf(fp, "\n");

    // Statistical analysis reporting
    fprintf(fp, "\n====== STATISTICAL ANALYSIS ======\n");
    fprintf(fp, "\n------------ Phase A ------------");
    fprintf(fp, "\nVariance:            %lf", *var_A);
    fprintf(fp, "\nStandard deviation:  %lf\n", *stddev_A);

    fprintf(fp, "\n------------ Phase B ------------");
    fprintf(fp, "\nVariance:            %lf", *var_B);
    fprintf(fp, "\nStandard deviation:  %lf\n", *stddev_B);

    fprintf(fp, "\n------------ Phase C ------------");
    fprintf(fp, "\nVariance:            %lf", *var_C);
    fprintf(fp, "\nStandard deviation:  %lf\n", *stddev_C);

    // Report clipping
    fprintf(fp, "\n====== CLIP DETECTION ======\n");
    int i = 0;
    size_t clip_count = 0;

    for (i = 0; i < SAMPLES; i++) {
        if (data[i].health_flags & (CLIP_A | CLIP_B | CLIP_C)) {

            clip_count++;
            fprintf(fp, "\nClipping detected at %lfs in phase(s): ", data[i].time);

            if (data[i].health_flags & CLIP_A) fprintf(fp, "A ");
            if (data[i].health_flags & CLIP_B) fprintf(fp, "B ");
            if (data[i].health_flags & CLIP_C) fprintf(fp, "C ");
        }
    }

    if (clip_count != 0) {
        fprintf(fp, "\nNumber of clipped samples: %zu\n", clip_count);
    }
    else fprintf(fp, "No clipping detected.");

    fclose(fp);

    return 0;
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

// Test function to report samples flagged as clipping
/* void report_clipping() {
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
} */