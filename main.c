#include "io.h"
#include "waveform.h"

int main(void) {

    // TODO: Validate successful function of load_data before continuing program run
    load_data();

    double rms_A, rms_B, rms_C;
    compute_rms(&rms_A, &rms_B, &rms_C);

    double p2p_A, p2p_B, p2p_C;
    compute_p2p(&p2p_A, &p2p_B, &p2p_C );

    double mean_A, mean_B, mean_C;
    compute_dc_offset(&mean_A, &mean_B, &mean_C);

    detect_clipping();

    int tolerance_status[3] = {0, 0, 0}; // Status indicators for A, B and C respectively
    check_rms_tolerance(tolerance_status, 0, &rms_A);
    check_rms_tolerance(tolerance_status, 1, &rms_B);
    check_rms_tolerance(tolerance_status, 2, &rms_C);

    // TODO: write test function
    double var_A, var_B, var_C;
    double stddev_A, stddev_B, stddev_C;
    compute_variance_std_dev(&var_A, &var_B, &var_C, &stddev_A, &stddev_B, &stddev_C);

    file_output(&rms_A, &rms_B, &rms_C, &p2p_A, &p2p_B, &p2p_C, &mean_A, &mean_B, &mean_C, tolerance_status, &var_A, &var_B, &var_C, &stddev_A, &stddev_B, &stddev_C);
    free_data();

    return 0;
}