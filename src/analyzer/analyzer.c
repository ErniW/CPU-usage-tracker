#include <analyzer.h>
#include <queue.h>
#include <stdbool.h>

extern volatile __sig_atomic_t exitFlag;
extern CPU_usage usageTracker;
extern Queue CPU_stateBuffer;
extern int NUM_CORES;

void* analyzerFunction(void* args){

    bool isInit = false;
   

    while(!exitFlag){

        sem_wait(&CPU_stateBuffer.full_sem);
        pthread_mutex_lock(&CPU_stateBuffer.access_mtx);

        CPU_state newData = Queue_pop(&CPU_stateBuffer);

        if(!isInit){
            free(usageTracker.current->cores);
            *usageTracker.prev->cores = *newData.cores;
            usageTracker.prev->total = newData.total;
            
        
            isInit = true;
        }
        
        
        *usageTracker.current = newData;

        usageTracker.total = CPU_getAverageUsage(&usageTracker.prev->total, &usageTracker.current->total);

        for(int i=0; i<NUM_CORES; i++){
            usageTracker.coreValue[i] = CPU_getAverageUsage(&usageTracker.prev->cores[i], &usageTracker.current->cores[i]);
        }

        pthread_mutex_unlock(&CPU_stateBuffer.access_mtx);
        sem_post(&CPU_stateBuffer.empty_sem);
    }
    
    sem_post(&CPU_stateBuffer.full_sem);
    pthread_exit(NULL);
}
