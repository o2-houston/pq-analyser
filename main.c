#include <stdio.h>
#include "io.h"
#include "waveform.h"

int main(void) {

    load_data();
    free_data();

    double rms_A, rms_B, rms_C;
    compute_rms(&rms_A, &rms_B, &rms_C);
    print_rms(&rms_A, &rms_B, &rms_C);

    double p2p_A, p2p_B, p2p_C;
    compute_p2p(&p2p_A, &p2p_B, &p2p_C );

    return 0;
}