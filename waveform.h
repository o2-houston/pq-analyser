#ifndef PQ_ANALYSER_WAVEFORM_H
#define PQ_ANALYSER_WAVEFORM_H
#include <stdint.h>

typedef struct {
    double time;
    double v_phA;
    double v_phB;
    double v_phC;
    double line_current;
    double frequency;
    double power_factor;
    double thd_percent;
    uint8_t health_flags;
} WaveformSample_t;

// Define bit positions for health flags
#define CLIP_A (1 << 0)
#define CLIP_B (1 << 1)
#define CLIP_C (1 << 2)
#define RESERVED_3 (1 << 3)
#define RESERVED_4 (1 << 4)
#define RESERVED_5 (1 << 5)
#define RESERVED_6 (1 << 6)
#define RESERVED_7 (1 << 7)

typedef struct {
    double rms[3];
    double p2p[3];
    double mean[3];
    int tolerance_status[3];
    double variance[3];
    double std_dev[3];
} WaveformAnalysis_t;

void compute_rms(WaveformAnalysis_t *analysis);
void compute_p2p(WaveformAnalysis_t *analysis);
void compute_dc_offset(WaveformAnalysis_t *analysis);
void detect_clipping();
void check_rms_tolerance(WaveformAnalysis_t *analysis, int index);
void compute_variance_std_dev(WaveformAnalysis_t *analysis);

#endif //PQ_ANALYSER_WAVEFORM_H