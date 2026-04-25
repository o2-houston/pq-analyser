#ifndef PQ_ANALYSER_IO_H
#define PQ_ANALYSER_IO_H

#include "waveform.h"

#define SAMPLES 1000

extern WaveformSample_t *data;

int load_data();
void free_data();

#endif //PQ_ANALYSER_IO_H