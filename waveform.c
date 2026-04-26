#include <stdio.h>
#include <math.h>
#include <float.h>
#include "waveform.h"
#include "io.h"

//int compute_rms() {}
    // Phase A

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
// int compute_dco(){}
// int detect_clipping(){}
// int check_tolerance(){}