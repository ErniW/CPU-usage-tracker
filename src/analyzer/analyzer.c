#include "analyzer.h"
#include <queue.h>
#include <watchdog.h>

extern CPU_usage usageTracker;
extern Queue CPU_stateBuffer;
extern int NUM_CORES;

void analyzerCleanup(void* args) {
    printf("Analyzer cleanup called\n");

    if(usageTracker.current != NULL){
        free(usageTracker.current->cores);
        free(usageTracker.current);
        usageTracker.current = NULL;
    } 

    if(usageTracker.prev != NULL) {
        free(usageTracker.prev->cores);
        free(usageTracker.prev);
        usageTracker.prev = NULL;
    }
   
      pthread_mutex_unlock(&CPU_stateBuffer.access_mtx);
}

void* analyzerFunction(void* args){
    CPU_state newData;
    (void)args;

    pthread_cleanup_push(analyzerCleanup, NULL)

    while(1){
        
        watchdogUpdate(ANALYZER_THREAD);

        sem_wait(&CPU_stateBuffer.full_sem);
        pthread_mutex_lock(&CPU_stateBuffer.access_mtx);
       
        newData = Queue_pop(&CPU_stateBuffer);

        if(usageTracker.prev == NULL){
            copy_CPU_state(&usageTracker.prev, &newData);
        }
        else if(usageTracker.current == NULL){
            
            copy_CPU_state(&usageTracker.current, &newData);
            usageTracker.total = CPU_getAverageUsage(&usageTracker.prev->total, &usageTracker.current->total);

            for(int i=0; i<NUM_CORES; i++){
                usageTracker.coreValue[i] = CPU_getAverageUsage(&usageTracker.prev->cores[i], &usageTracker.current->cores[i]);
            }
         
        }

        pthread_mutex_unlock(&CPU_stateBuffer.access_mtx);
        sem_post(&CPU_stateBuffer.empty_sem);    
    }

    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}
