#ifndef PQ_ANALYSER_IO_H
#define PQ_ANALYSER_IO_H

#include "waveform.h"

extern WaveformSample_t *data;
extern size_t sample_count;

int load_data();
void free_data();
int verify_input_status(int status);
int file_output(const WaveformAnalysis_t *analysis);
int output_sort(SortedData_t sorted_data);
#endif //PQ_ANALYSER_IO_H