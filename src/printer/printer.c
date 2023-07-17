#include "printer.h"
#include <queue.h>
#include <watchdog.h>

extern CPU_usage usageTracker;
extern Queue CPU_stateBuffer;
extern int NUM_CORES;

void printerCleanup(void* args){
    (void)args;

    pthread_mutex_unlock(&CPU_stateBuffer.access_mtx);

    #ifdef DEBUG
        printf("Printer cleanup done\n");
    #endif
}

void* printerFunction(void* args){
    (void)args;

    pthread_cleanup_push(printerCleanup, NULL)
   
    while(1){

        watchdogUpdate(PRINTER_THREAD);

        pthread_mutex_lock(&CPU_stateBuffer.access_mtx);

        if(usageTracker.prev != NULL  && usageTracker.current != NULL){

            printf("CPU USAGE TRACKER\n");
            printf(COLOR_INVERSED "Total: %3lu%%\n" COLOR_CLEAR, usageTracker.total);        

            for(int i=0; i<NUM_CORES; i++){

                unsigned int usage = usageTracker.coreValue[i];

                char* color = setColor(usage);

                printf("CPU %2d: ", i+1);
                printUsageBar(usage, color);
                printf( "%s%3u%%" COLOR_CLEAR "\n", color, usage);
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
   
    pthread_cleanup_pop(1);

    pthread_exit(NULL);
}

char* setColor(unsigned int usage){
    if(usage < 40)
        return COLOR_GREEN;
    else if(usage < 80) 
        return COLOR_YELLOW;
    else
        return COLOR_RED;
}

void printUsageBar(unsigned int usage, char* color){
    unsigned long barLength = usage / 10;
    char bar[11] = "";

    if(usage >= 5 && usage != 100) barLength += 1;

    memset(bar, ' ', sizeof(bar)-1);
    memset(bar, '=', barLength);

    printf("[%s%10s" COLOR_CLEAR "]", color, bar);
}
