#include "printer.h"
#include <queue.h>

extern volatile __sig_atomic_t exitFlag;
extern CPU_usage usageTracker;
extern Queue CPU_stateBuffer;
extern int NUM_CORES;


void* printerFunction(void* args){

    while(!exitFlag){

        pthread_mutex_lock(&CPU_stateBuffer.access_mtx);

        printf("%u - \t", usageTracker.total);

        for(int i=0; i<NUM_CORES; i++){
            printf("%u \t", usageTracker.coreValue[i]);
            usageTracker.prev->cores[i] = usageTracker.current->cores[i];
        }
        printf("\n");

        usageTracker.prev->total = usageTracker.current->total;

        pthread_mutex_unlock(&CPU_stateBuffer.access_mtx);
       
        sleep(1);
    }

    pthread_exit(NULL);
}
