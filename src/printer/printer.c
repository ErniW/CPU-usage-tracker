#include "printer.h"
#include <queue.h>
#include <watchdog.h>

extern CPU_usage usage;
extern Queue buffer;
extern int NUM_CORES;

void printerCleanup(void* args){
    (void)args;

    pthread_mutex_unlock(&buffer.access_mtx);

    #ifdef DEBUG
        printf("Printer cleanup done\n");
    #endif
}

void* printerFunction(void* args){
    (void)args;

    pthread_cleanup_push(printerCleanup, NULL)
   
    while(1){

        watchdogUpdate(PRINTER_THREAD);

        pthread_mutex_lock(&buffer.access_mtx);

        if(usage.prev != NULL  && usage.current != NULL){

            printf("CPU USAGE TRACKER\n");
            printf(COLOR_INVERSED "Total: %3lu%%\n" COLOR_CLEAR, usage.total);        

            for(int i=0; i<NUM_CORES; i++){

                unsigned int value = usage.value[i];

                char* color = setColor(value);

                printf("CPU %2d: ", i+1);
                printUsageBar(value, color);
                printf( "%s%3u%%" COLOR_CLEAR "\n", color, value);
            }
            
            printf(CLEAR_SCREEN);
             
            free(usage.prev->cores);
            free(usage.prev);
            usage.prev = usage.current;
            usage.current = NULL;
        }
        
        pthread_mutex_unlock(&buffer.access_mtx); 
        sleep(1);
    }
   
    pthread_cleanup_pop(1);

    pthread_exit(NULL);
}

char* setColor(unsigned int value){
    if(value < 40)
        return COLOR_GREEN;
    else if(value < 80) 
        return COLOR_YELLOW;
    else
        return COLOR_RED;
}

void printUsageBar(unsigned int value, char* color){
    unsigned long barLength = value / 10;
    char bar[11] = "";

    if(value >= 5 && value != 100) 
        barLength += 1;

    memset(bar, ' ', sizeof(bar)-1);
    memset(bar, '=', barLength);

    printf("[%s%10s" COLOR_CLEAR "]", color, bar);
}
