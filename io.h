#ifndef PQ_ANALYSER_IO_H
#define PQ_ANALYSER_IO_H

#include "waveform.h"

// TODO: dynamically define value of SAMPLES
#define SAMPLES 1000

extern WaveformSample_t *data;

int load_data();
void free_data();
int file_output(WaveformAnalysis_t *analysis);

#endif //PQ_ANALYSER_IO_H