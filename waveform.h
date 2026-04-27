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

void compute_rms(double *rms_A, double *rms_B, double *rms_C);
void compute_p2p(double *p2p_A, double *p2p_B, double *p2p_C);
void compute_dc_offset(double *mean_A, double *mean_B, double *mean_C);
void detect_clipping();
void check_rms_tolerance(double *tolerance_status, int index, double* rms_value);
void compute_variance_std_dev(double *var_A, double *var_B, double *var_C, double *stddev_A, double *stddev_B, double *stddev_C);

#endif //PQ_ANALYSER_WAVEFORM_H