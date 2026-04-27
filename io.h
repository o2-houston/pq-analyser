#ifndef PQ_ANALYSER_IO_H
#define PQ_ANALYSER_IO_H

#include "waveform.h"

// TODO: dynamically define value of SAMPLES
#define SAMPLES 1000

extern WaveformSample_t *data;

int load_data();
void free_data();
void report_clipping();

// int print_rms(double *rms_A, double *rms_B, double *rms_C);
// int print_p2p(double *p2p_A, double *p2p_B, double *p2p_C);
void print_dc_offset(double *mean_A, double *mean_B, double *mean_C);
void print_tolerance_status(double* tolerance_status);

#endif //PQ_ANALYSER_IO_H