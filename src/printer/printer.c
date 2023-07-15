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
            printf(COLOR_INVERSED "Total: %3lu%%\n" COLOR_CLEAR, usageTracker.total);        

            for(int i=0; i<NUM_CORES; i++){
                printCoreStats(i, usageTracker.coreValue[i]);
            }
            
            printf(CLEAR_SCREEN);

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

void printCoreStats(int i, unsigned int usage){
    unsigned long barLength = usage / 10;
    char color[6] = "";
    char bar[11] = "";

    if(usage >= 5 && usage != 100) barLength += 1;

    memset(bar, ' ', sizeof(bar)-1);
    memset(bar, '=', barLength);

    if(barLength <= 3)      strcpy(color, COLOR_GREEN);
    else if(barLength <=7)  strcpy(color, COLOR_YELLOW);
    else                    strcpy(color, COLOR_RED);

    printf("CPU %2d: ", i+1);
    printf("[%s%10s" COLOR_CLEAR "]", color, bar);
    printf( "%s%3u%%" COLOR_CLEAR "\n", color, usage);
}
