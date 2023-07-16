#include "sigint.h"

extern pthread_t readerThread;
extern pthread_t analyzerThread;
extern pthread_t printerThread;
extern pthread_t watchdogThread;

void SIGINTHandler(int signal) {
    printf("[SIGINT %d] received. Exiting...\n", signal);
    // exitFlag = 1;
    // pthread_mutex_unlock(&watchdog_mtx);

    pthread_cancel(watchdogThread);
    pthread_cancel(printerThread);
    pthread_cancel(analyzerThread);   
    pthread_cancel(readerThread);     
}
