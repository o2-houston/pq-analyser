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

void free_data(void) {
    free(data);  // Free dynamically allocated memory
    data = NULL;
}

// TODO: add total samples read, date/time of export
int file_output(WaveformAnalysis_t *analysis) {

    // Validate file output
    FILE *fp = fopen("output.txt", "w");
    if (fp == NULL) {
        printf("Error opening output file.");
        return 1;
    }

    fprintf(fp, "====== RMS VALUES ======\n");
    fprintf(fp, "\nRMS for A: %lfV", analysis->rms[0]);
    fprintf(fp, "\nRMS for B: %lfV", analysis->rms[1]);
    fprintf(fp, "\nRMS for C: %lfV\n", analysis->rms[2]);

    fprintf(fp, "\n====== PEAK-TO-PEAK VALUES ======\n");
    fprintf(fp, "\nPeak-to-peak voltage for A: %lfV", analysis->p2p[0]);
    fprintf(fp, "\nPeak-to-peak voltage for B: %lfV", analysis->p2p[1]);
    fprintf(fp, "\nPeak-to-peak voltage for C: %lfV\n", analysis->p2p[2]);

    fprintf(fp, "\n====== DC OFFSET VALUES ======\n");
    fprintf(fp, "\nDC offset for A: %lfV", analysis->mean[0]);
    fprintf(fp, "\nDC offset for B: %lfV", analysis->mean[1]);
    fprintf(fp, "\nDC offset for C: %lfV\n", analysis->mean[2]);

    fprintf(fp, "\n====== VOLTAGE TOLERANCE STATUS ======\n");

    // Phase A tolerance
    fprintf(fp, "\nPhase A: ");
    switch (analysis->tolerance_status[0]) {
        case 1: fprintf(fp, "Above tolerance threshold."); break;
        case 0: fprintf(fp, "Within tolerance threshold."); break;
        case -1: fprintf(fp, "Below tolerance threshold."); break;
        default : fprintf(fp, "Unable to read tolerance status.");
    }

    // Phase B tolerance
    fprintf(fp, "\nPhase B: ");
    switch (analysis->tolerance_status[0]) {
        case 1: fprintf(fp, "Above tolerance threshold."); break;
        case 0: fprintf(fp, "Within tolerance threshold."); break;
        case -1: fprintf(fp, "Below tolerance threshold."); break;
        default : fprintf(fp, "Unable to read tolerance status.");
    }

    // Phase C tolerance
    fprintf(fp, "\nPhase C: ");
    switch (analysis->tolerance_status[0]) {
        case 1: fprintf(fp, "Above tolerance threshold."); break;
        case 0: fprintf(fp, "Within tolerance threshold."); break;
        case -1: fprintf(fp, "Below tolerance threshold."); break;
        default : fprintf(fp, "Unable to read tolerance status.");
    }

    fprintf(fp, "\n");

    // Statistical analysis reporting
    fprintf(fp, "\n====== STATISTICAL ANALYSIS ======\n");
    fprintf(fp, "\n------------ Phase A ------------");
    fprintf(fp, "\nVariance:            %lf", analysis->variance[0]);
    fprintf(fp, "\nStandard deviation:  %lf\n", analysis->std_dev[0]);

    fprintf(fp, "\n------------ Phase B ------------");
    fprintf(fp, "\nVariance:            %lf", analysis->variance[1]);
    fprintf(fp, "\nStandard deviation:  %lf\n", analysis->std_dev[1]);

    fprintf(fp, "\n------------ Phase C ------------");
    fprintf(fp, "\nVariance:            %lf", analysis->variance[2]);
    fprintf(fp, "\nStandard deviation:  %lf\n", analysis->std_dev[2]);

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