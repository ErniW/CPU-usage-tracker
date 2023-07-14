#include "printer.h"
#include <queue.h>

extern volatile __sig_atomic_t exitFlag;
extern CPU_usage usageTracker;
extern Queue CPU_stateBuffer;
extern int NUM_CORES;

void* printerFunction(void* args){

    while(!exitFlag){

        pthread_mutex_lock(&CPU_stateBuffer.access_mtx);

        if(usageTracker.prev != NULL  && usageTracker.current != NULL){

            free(usageTracker.prev->cores);
            free(usageTracker.prev);

            printf("Total: %u ", usageTracker.total);

            for(int i=0; i<NUM_CORES; i++){
                printf("CPU %d: %u ", i, usageTracker.coreValue[i]);          
            }

            printf("\n");

            usageTracker.prev = usageTracker.current;
            usageTracker.current = NULL;
        }

        pthread_mutex_unlock(&CPU_stateBuffer.access_mtx);
       
        sleep(1);
    }

    pthread_exit(NULL);
}