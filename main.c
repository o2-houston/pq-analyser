#include <stdio.h>
#include "io.h"
#include "waveform.h"

int main(void) {

    load_data();
    free_data();

    double p2p_A, p2p_B, p2p_C;
    compute_p2p(&p2p_A, &p2p_B, &p2p_C );

    print_p2p(&p2p_A, &p2p_B, &p2p_C);

    return 0;
}