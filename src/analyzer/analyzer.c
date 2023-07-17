#include "analyzer.h"
#include <queue.h>
#include <watchdog.h>

extern CPU_usage usage;
extern Queue buffer;
extern int NUM_CORES;

void analyzerCleanup(void* args) {
    (void)args;

    if(usage.current != NULL){
        free(usage.current->cores);
        free(usage.current);
        usage.current = NULL;
    } 

    if(usage.prev != NULL) {
        free(usage.prev->cores);
        free(usage.prev);
        usage.prev = NULL;
    }
   
    free(usage.value);
    pthread_mutex_unlock(&buffer.access_mtx);
    sem_destroy(&buffer.full_sem);

    #ifdef DEBUG
        printf("Analyzer cleanup done\n");
    #endif
}

void* analyzerFunction(void* args){
    CPU_state newData;
    (void)args;

    pthread_cleanup_push(analyzerCleanup, NULL)

    while(1){
        
        watchdogUpdate(ANALYZER_THREAD);

        sem_wait(&buffer.full_sem);
        pthread_mutex_lock(&buffer.access_mtx);
       
        newData = Queue_pop(&buffer);

        if(usage.prev == NULL){
            copy_CPU_state(&usage.prev, &newData);
        }
        else if(usage.current == NULL){
            
            copy_CPU_state(&usage.current, &newData);
            usage.total = CPU_getAverageUsage(&usage.prev->total, &usage.current->total);

            for(int i=0; i<NUM_CORES; i++){
                usage.value[i] = CPU_getAverageUsage(&usage.prev->cores[i], &usage.current->cores[i]);
            }
            
        }

        pthread_mutex_unlock(&buffer.access_mtx);
        sem_post(&buffer.empty_sem);    
    }

    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}
