#ifndef PQ_ANALYSER_WAVEFORM_H
#define PQ_ANALYSER_WAVEFORM_H

typedef struct {
    double time;
    double v_phA;
    double v_phB;
    double v_phC;
    double line_current;
    double frequency;
    double power_factor;
    double thd_percent;
} WaveformSample_t;

void compute_rms(double *rms_A, double *rms_B, double *rms_C);
void compute_p2p(double *p2p_A, double *p2p_B, double *p2p_C);
// int compute_dco();
// int detect_clipping();
// int check_tolerance();

#endif //PQ_ANALYSER_WAVEFORM_H