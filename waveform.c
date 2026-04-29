#include <stdio.h>
#include <math.h>
#include <float.h>
#include "waveform.h"
#include "io.h"

// Hard limit of sensor - voltage samples above this value will be flagged as clipped
#define CLIP_LIMIT 324.9

// Parameters for tolerance check
#define TOL_LIMIT_UPPER 253
#define TOL_LIMIT_LOWER 207

void compute_rms(WaveformAnalysis_t *analysis) {

    int i = 0;
    double sum_sq_A = 0,  sum_sq_B = 0, sum_sq_C = 0;

    for (i=0; i < sample_count; i++) {
        sum_sq_A += pow(data[i].v_phA, 2);
        sum_sq_B += pow(data[i].v_phB, 2);
        sum_sq_C += pow(data[i].v_phC, 2);
    }

    analysis->rms[0] = sqrt(sum_sq_A/(double)sample_count);
    analysis->rms[1] = sqrt(sum_sq_B/(double)sample_count);
    analysis->rms[2] = sqrt(sum_sq_C/(double)sample_count);
}

// TODO: Add data validation
void compute_p2p(WaveformAnalysis_t *analysis) {

    int i = 0;
    double min_A = DBL_MAX, max_A = -DBL_MAX;
    double min_B = DBL_MAX, max_B = -DBL_MAX;
    double min_C = DBL_MAX, max_C = -DBL_MAX;

    for (i = 0; i < sample_count; i++) {
        if (data[i].v_phA < min_A) min_A = data[i].v_phA;
        if (data[i].v_phA > max_A) max_A = data[i].v_phA;
        if (data[i].v_phB < min_B) min_B = data[i].v_phB;
        if (data[i].v_phB > max_B) max_B = data[i].v_phB;
        if (data[i].v_phC < min_C) min_C = data[i].v_phC;
        if (data[i].v_phC > max_C) max_C = data[i].v_phC;
    }
        analysis->p2p[0] = max_A - min_A;
        analysis->p2p[1] = max_B - min_B;
        analysis->p2p[2] = max_C - min_C;
}

// TODO: add validation to avoid dividing by zero
// TODO: handle floating-point rounding
void compute_dc_offset(WaveformAnalysis_t *analysis) {

    int i = 0;
    double sum_A = 0, sum_B = 0, sum_C = 0;

    for (i = 0; i < sample_count; i++) {
        sum_A += data[i].v_phA;
        sum_B += data[i].v_phB;
        sum_C += data[i].v_phC;
    }

    analysis->mean[0] = sum_A/(double)sample_count;
    analysis->mean[1] = sum_B/(double)sample_count;
    analysis->mean[2] = sum_C/(double)sample_count;
}

void detect_clipping() {
    int i = 0;

    for (i = 0; i < sample_count; i++) {
        if (fabs(data[i].v_phA) > CLIP_LIMIT){
            data[i].health_flags |= CLIP_A;}
        if (fabs(data[i].v_phB) > CLIP_LIMIT){
            data[i].health_flags |= CLIP_B;}
        if (fabs(data[i].v_phC) > CLIP_LIMIT){
            data[i].health_flags |= CLIP_C;}
    }
}

void check_rms_tolerance(WaveformAnalysis_t *analysis, int index) {

    if (analysis->rms[index] > TOL_LIMIT_UPPER) {
        analysis->tolerance_status[index] = 1; // Above tolerance threshold
    }
    else if (analysis->rms[index] < TOL_LIMIT_LOWER) {
        analysis->tolerance_status[index] = -1; // Below tolerance threshold
    }
    else analysis->tolerance_status[index] = 0; // Within tolerance
}

void compute_range(WaveformAnalysis_t *analysis, const MetricType metric) {
    int i = 0;
    double min_value = DBL_MAX, max_value = -DBL_MAX;

    for (i = 0; i < sample_count; i++) {
        double field_value = 0;

        switch (metric) {
            case TIME: field_value = data[i].time; break;
            case LINE_CURRENT: field_value = data[i].line_current; break;
            case FREQUENCY: field_value = data[i].frequency; break;
            case POWER_FACTOR: field_value = data[i].power_factor; break;
            case THD_PERCENT: field_value = data[i].thd_percent; break;
            default: printf("Error specifying metric type for range analysis.");
        }

        if (field_value < min_value) min_value = field_value;
        if (field_value > max_value) max_value = field_value;
    }
    switch (metric) {
        case TIME: {
            analysis->range_time[0] = min_value;
            analysis->range_time[1] = max_value;
            analysis->range_time[2] = max_value - min_value;
        } break;
        case LINE_CURRENT: {
            analysis->range_line_current[0] = min_value;
            analysis->range_line_current[1] = max_value;
            analysis->range_line_current[2] = max_value - min_value;
        } break;
        case FREQUENCY: {
            analysis->range_freq[0] = min_value;
            analysis->range_freq[1] = max_value;
            analysis->range_freq[2] = max_value - min_value;
        } break;
        case POWER_FACTOR: {
            analysis->range_power_factor[0] = min_value;
            analysis->range_power_factor[1] = max_value;
            analysis->range_power_factor[2] = max_value - min_value;
        } break;
        case THD_PERCENT: {
            analysis->range_thd[0] = min_value;
            analysis->range_thd[1] = max_value;
            analysis->range_thd[2] = max_value - min_value;
        } break;
        default: printf("Error computing range for metric %d.", metric);
    }
}

void compute_variance_std_dev(WaveformAnalysis_t *analysis) {

    int i = 0;
    double sum_A = 0, sum_B = 0, sum_C = 0;

    for (i = 0; i < sample_count; i++) {
        sum_A += data[i].v_phA;
        sum_B += data[i].v_phB;
        sum_C += data[i].v_phC;
    }

    double mean_A = sum_A/(double)sample_count;
    double mean_B = sum_B/(double)sample_count;
    double mean_C = sum_C/(double)sample_count;

    double sum_sq_diff_A = 0, sum_sq_diff_B = 0, sum_sq_diff_C = 0;

    for (i = 0; i < sample_count; i++) {
        sum_sq_diff_A += pow(data[i].v_phA - mean_A, 2);
        sum_sq_diff_B += pow(data[i].v_phB - mean_B, 2);
        sum_sq_diff_C += pow(data[i].v_phC - mean_C, 2);
    }

    analysis->variance[0] = sum_sq_diff_A/((double)sample_count - 1);
    analysis->variance[1] = sum_sq_diff_B/((double)sample_count - 1);
    analysis->variance[2] = sum_sq_diff_C/((double)sample_count - 1);

    analysis->std_dev[0] = sqrt(analysis->variance[0]);
    analysis->std_dev[1] = sqrt(analysis->variance[1]);
    analysis->std_dev[2] = sqrt(analysis->variance[2]);
}