#include <math.h>
#include <float.h>
#include "waveform.h"
#include "io.h"

// Hard limit of sensor - voltage samples above this value will be flagged as clipped
#define CLIP_LIMIT 324.9

// Parameters for tolerance check
#define TOL_LIMIT_UPPER 253
#define TOL_LIMIT_LOWER 207

void compute_rms(double *rms_A, double *rms_B, double *rms_C) {

    int i = 0;
    double sum_sq_A = 0,  sum_sq_B = 0, sum_sq_C = 0;

    for (i=0; i < SAMPLES; i++) {
        sum_sq_A += pow(data[i].v_phA, 2);
        sum_sq_B += pow(data[i].v_phB, 2);
        sum_sq_C += pow(data[i].v_phC, 2);
    }

    *rms_A = sqrt(sum_sq_A/SAMPLES);
    *rms_B = sqrt(sum_sq_B/SAMPLES);
    *rms_C = sqrt(sum_sq_C/SAMPLES);
}

// TODO: Add data validation,
void compute_p2p(double *p2p_A, double *p2p_B, double *p2p_C) {

    int i = 0;
    double min_A = DBL_MAX, max_A = -DBL_MAX;
    double min_B = DBL_MAX, max_B = -DBL_MAX;
    double min_C = DBL_MAX, max_C = -DBL_MAX;

    for (i = 0; i < SAMPLES; i++) {
        if (data[i].v_phA < min_A) min_A = data[i].v_phA;
        if (data[i].v_phA > max_A) max_A = data[i].v_phA;
        if (data[i].v_phB < min_B) min_B = data[i].v_phB;
        if (data[i].v_phB > max_B) max_B = data[i].v_phB;
        if (data[i].v_phC < min_C) min_C = data[i].v_phC;
        if (data[i].v_phC > max_C) max_C = data[i].v_phC;
    }

        *p2p_A = max_A - min_A;
        *p2p_B = max_B - min_B;
        *p2p_C = max_C - min_C;
}

// TODO: add validation to avoid dividing by zero
// TODO: handle floating-point rounding
void compute_dc_offset(double *mean_A, double *mean_B, double *mean_C) {

    int i = 0;
    double sum_A = 0, sum_B = 0, sum_C = 0;

    for (i = 0; i < SAMPLES; i++) {
        sum_A += data[i].v_phA;
        sum_B += data[i].v_phB;
        sum_C += data[i].v_phC;
    }

    *mean_A = sum_A/SAMPLES;
    *mean_B = sum_B/SAMPLES;
    *mean_C = sum_C/SAMPLES;
}

void detect_clipping() {
    int i = 0;

    for (i = 0; i < SAMPLES; i++) {
        if (data[i].v_phA > CLIP_LIMIT){
            data[i].health_flags |= CLIP_A;}
        if (data[i].v_phB > CLIP_LIMIT){
            data[i].health_flags |= CLIP_B;}
        if (data[i].v_phC > CLIP_LIMIT){
            data[i].health_flags |= CLIP_C;}
    }
}

void check_rms_tolerance(double* tolerance_status, int index, double* rms_value) {

    if (*rms_value > TOL_LIMIT_UPPER) {
        tolerance_status[index] = 1; // Above tolerance threshold
    }
    else if (*rms_value < TOL_LIMIT_LOWER) {
        tolerance_status[index] = -1; // Below tolerance threshold
    }
    else tolerance_status[index] = 0; // Within tolerance
}

// TODO: write test function
void compute_variance_std_dev(double *var_A, double *var_B, double *var_C, double *stddev_A, double *stddev_B, double *stddev_C) {

    int i = 0;
    double sum_A = 0, sum_B = 0, sum_C = 0;

    for (i = 0; i < SAMPLES; i++) {
        sum_A += data[i].v_phA;
        sum_B += data[i].v_phB;
        sum_C += data[i].v_phC;
    }

    double mean_A = sum_A/SAMPLES;
    double mean_B = sum_B/SAMPLES;
    double mean_C = sum_C/SAMPLES;

    double sum_sq_diff_A = 0, sum_sq_diff_B = 0, sum_sq_diff_C = 0;

    for (i = 0; i < SAMPLES; i++) {
        sum_sq_diff_A += pow(data[i].v_phA - mean_A, 2);
        sum_sq_diff_B += pow(data[i].v_phB - mean_B, 2);
        sum_sq_diff_C += pow(data[i].v_phC - mean_C, 2);
    }

    *var_A = sum_sq_diff_A/SAMPLES;
    *var_B = sum_sq_diff_B/SAMPLES;
    *var_C = sum_sq_diff_C/SAMPLES;

    *stddev_A = sqrt(*var_A);
    *stddev_B = sqrt(*var_B);
    *stddev_C = sqrt(*var_C);
}