#include "printer.h"
#include <queue.h>

extern volatile __sig_atomic_t exitFlag;
extern CPU_usage usageTracker;
extern Queue CPU_stateBuffer;
extern int NUM_CORES;

void* printerFunction(void* args){
    (void)args;
    while(!exitFlag){

        pthread_mutex_lock(&CPU_stateBuffer.access_mtx);

        if(usageTracker.prev != NULL  && usageTracker.current != NULL){
            
            printf("CPU USAGE TRACKER\n");
            printf("\x1b[7m");
            printf("Total: %3lu%%\n", usageTracker.total);
            printf("\x1b[0m");

            for(int i=0; i<NUM_CORES; i++){
                char bar[10];
                unsigned long barLength = usageTracker.coreValue[i] / 10;
                memset(bar, ' ', sizeof(bar));
                memset(bar, '=', barLength * sizeof(char));

                printf("CPU %2d: [%10s] %3u%% \n", i + 1, bar, usageTracker.coreValue[i]);
            }
            
            printf("\033[H\033[J");

            free(usageTracker.prev->cores);
            free(usageTracker.prev);
            usageTracker.prev = usageTracker.current;
            usageTracker.current = NULL;
        }

        pthread_mutex_unlock(&CPU_stateBuffer.access_mtx);
       
        sleep(1);
    }

    pthread_exit(NULL);
}
