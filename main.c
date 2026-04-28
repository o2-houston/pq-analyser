#include "io.h"
#include "waveform.h"

int main(void) {

    // TODO: Validate successful function of load_data before continuing program run
    load_data();

    WaveformAnalysis_t analysis;

    compute_rms(&analysis);
    compute_p2p(&analysis);
    compute_dc_offset(&analysis);
    detect_clipping();

    check_rms_tolerance(&analysis, 0);
    check_rms_tolerance(&analysis, 1);
    check_rms_tolerance(&analysis, 2);

    compute_variance_std_dev(&analysis);

    file_output(&analysis);
    free_data();

    return 0;
}