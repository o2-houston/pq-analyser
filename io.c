#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "waveform.h"

WaveformSample_t *data = NULL;
size_t sample_count = 0;

int load_data(void) {
    size_t count = 0;
    char header[256];
    int status = 0;

    // Opens file in program root
    FILE *fp = fopen("power_quality_log.csv", "r");

    // Validate input
    if (fp == NULL) {
        printf("Error opening input file.\n");
        status = 1; // File open failure
        goto cleanup;
    }

    // Read and discard the header line
    if (fgets(header, sizeof(header), fp) == NULL) {
        printf("Error reading header.\n");
        status = 3; // Header failure
        goto cleanup;
    }

    while (fgets(header, sizeof(header),fp)) {
        count++;
    }

    sample_count = count;

    fseek(fp, 0, SEEK_SET);
    fgets(header, sizeof(header), fp);

    if (sample_count > 0) {
        data = calloc(sample_count, sizeof(WaveformSample_t));

        if (data == NULL) {
            printf("Memory allocation failed.\n");
            status = 2;
            goto cleanup;
        }
    } else data = NULL;

    count = 0;

    // Read data from file
    int ret = 0;
    while (count < sample_count) {
        ret = fscanf(fp, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
            &data[count].time,
            &data[count].v_phA,
            &data[count].v_phB,
            &data[count].v_phC,
            &data[count].line_current,
            &data[count].frequency,
            &data[count].power_factor,
            &data[count].thd_percent);

        if (ret == 8) {
            count++;
        }
        else if (ret == EOF) {
            break;
        }
        else {
            printf("Malformed data at line %zu.\n", count + 2);
            break;
        }
    }

    sample_count = count;

    if (count == 0) {
        printf("No valid samples found.\n");
        status =  5; // Parsing failure
        goto cleanup;
    }

    cleanup:
        if (fp) fclose(fp);
        if (status != 0) {
            free(data);
            data = NULL;
            sample_count = 0;
        }

    return status;
}

void free_data(void) {
    free(data);  // Free dynamically allocated memory
    data = NULL;
}

int verify_input_status(int status) {

    if (load_data() != 0) {
        printf("Function load_data failed with status code: %d.", status);
        exit(status);
    }

    return 0;
}

int file_output(const WaveformAnalysis_t *analysis) {

    // Validate file output
    FILE *fp = fopen("results.txt", "w");
    if (fp == NULL) {
        printf("Error opening output file.");
        return 1;
    }

    fprintf(fp, "Number of samples read: %zu\n\n", sample_count);

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
    switch (analysis->tolerance_status[1]) {
        case 1: fprintf(fp, "Above tolerance threshold."); break;
        case 0: fprintf(fp, "Within tolerance threshold."); break;
        case -1: fprintf(fp, "Below tolerance threshold."); break;
        default : fprintf(fp, "Unable to read tolerance status.");
    }

    // Phase C tolerance
    fprintf(fp, "\nPhase C: ");
    switch (analysis->tolerance_status[2]) {
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

    fprintf(fp, "\n====== SAMPLE RANGES ======\n");
    fprintf(fp, "\nTime:\n");
    fprintf(fp, "%lfs - %lfs\n", analysis->range_time[0], analysis->range_time[1]);

    fprintf(fp, "\nLine current:\n");
    fprintf(fp, "%lfA - %lfA\n", analysis->range_line_current[0], analysis->range_line_current[1]);

    fprintf(fp, "\nFrequency:\n");
    fprintf(fp, "%lfHz - %lfHz\n", analysis->range_freq[0], analysis->range_freq[1]);

    fprintf(fp, "\nPower factor:\n");
    fprintf(fp, "%lf - %lf\n", analysis->range_power_factor[0], analysis->range_power_factor[1]);

    fprintf(fp, "\nTHD percentage:\n");
    fprintf(fp, "%lf - %lf\n", analysis->range_thd[0], analysis->range_thd[1]);


    // Report clipping
    fprintf(fp, "\n====== CLIP DETECTION ======\n");
    int i = 0;
    size_t clip_count[3] = {0, 0, 0};

    for (i = 0; i < sample_count; i++) {
        if (data[i].health_flags & (CLIP_A | CLIP_B | CLIP_C)) {
            fprintf(fp, "\nClipping detected at %lfs in phase(s): ", data[i].time);

            if (data[i].health_flags & CLIP_A) {
                fprintf(fp, "A ");
                ++clip_count[0];
            }
            if (data[i].health_flags & CLIP_B) {
                fprintf(fp, "B ");
                ++clip_count[1];
            }
            if (data[i].health_flags & CLIP_C) {
                fprintf(fp, "C ");
                ++clip_count[2];
            }
        }
    }

    if (clip_count[0] != 0) {
        fprintf(fp, "\n\n%zu clipped samples detected in A.\n", clip_count[0]);
    }
    else fprintf(fp, "\n\nNo clipping detected in phase A.\n");

    if (clip_count[1] != 0) {
        fprintf(fp, "%zu clipped samples detected in B.\n", clip_count[1]);
    }
    else fprintf(fp, "No clipping detected in phase B.\n");

    if (clip_count[2] != 0) {
        fprintf(fp, "%zu clipped samples detected in C.\n", clip_count[2]);
    }
    else fprintf(fp, "No clipping detected in phase C.\n");

    fclose(fp);

    return 0;
}