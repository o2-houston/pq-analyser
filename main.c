#include "io.h"
#include "waveform.h"

int main(void) {

    // TODO: Validate successful function of load_data before continuing program run
    int status = load_data();
    verify_input_status(status);

    WaveformAnalysis_t analysis;

    compute_rms(&analysis);
    compute_p2p(&analysis);
    compute_dc_offset(&analysis);
    detect_clipping();
    compute_variance_std_dev(&analysis);

    check_rms_tolerance(&analysis, 0);
    check_rms_tolerance(&analysis, 1);
    check_rms_tolerance(&analysis, 2);

    compute_range(&analysis, TIME);
    compute_range(&analysis, LINE_CURRENT);
    compute_range(&analysis, FREQUENCY);
    compute_range(&analysis, POWER_FACTOR);
    compute_range(&analysis, THD_PERCENT);

    SortedData_t *sorted_data = allocate_sort_memory();
    sort_samples(sorted_data);

    file_output(&analysis);
    output_sort(*sorted_data);

    free_data();

    return 0;
}