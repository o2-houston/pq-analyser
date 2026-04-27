#include "io.h"
#include "waveform.h"

int main(void) {

    // TODO: Validate successful function of load_data before continuing program run
    load_data();
    free_data();

    double rms_A, rms_B, rms_C;
    compute_rms(&rms_A, &rms_B, &rms_C);

    double p2p_A, p2p_B, p2p_C;
    compute_p2p(&p2p_A, &p2p_B, &p2p_C );

    double mean_A, mean_B, mean_C;
    compute_dc_offset(&mean_A, &mean_B, &mean_C);
    print_dc_offset(&mean_A, &mean_B, &mean_C);

    detect_clipping();
    report_clipping();

    double tolerance_status[3] = {0, 0, 0}; // Status indicators for A, B and C respectively
    check_rms_tolerance(tolerance_status, 0, &rms_A);
    check_rms_tolerance(tolerance_status, 1, &rms_B);
    check_rms_tolerance(tolerance_status, 2, &rms_C);
    print_tolerance_status(tolerance_status);

    return 0;
}