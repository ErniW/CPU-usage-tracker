#include "utils.h"
#include <signal.h>

extern volatile __sig_atomic_t exitFlag;
volatile __sig_atomic_t exitFlag = 0;

void handleSIGINT(int signal) {
    printf("[SIGINT %d] received. Exiting...\n", signal);
    exitFlag = 1;
}
