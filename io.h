#ifndef PQ_ANALYSER_IO_H
#define PQ_ANALYSER_IO_H

#include "waveform.h"

#define SAMPLES 1000

extern WaveformSample_t *data;

int load_data();
void free_data();
int print_p2p(double *p2p_A, double *p2p_B, double *p2p_C);

#endif //PQ_ANALYSER_IO_H