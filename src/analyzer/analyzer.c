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
        CPU_stateBuffer.head = (CPU_stateBuffer.head + 1) % BUFFER_SIZE;

        if(!isInit){

            memcpy(usageTracker.prev->cores, CPU_stateBuffer.buffer[CPU_stateBuffer.head].cores, 8 * sizeof(CPU_core));
            usageTracker.prev->total = CPU_stateBuffer.buffer[CPU_stateBuffer.head].total;
            isInit = true;
        }

        memcpy(usageTracker.current->cores, CPU_stateBuffer.buffer[CPU_stateBuffer.head].cores, 8 * sizeof(CPU_core));

        usageTracker.total = CPU_getAverageUsage(&usageTracker.prev->total, &usageTracker.current->total);

        for(int i=0; i<NUM_CORES; i++){
            usageTracker.coreValue[i] = CPU_getAverageUsage(&usageTracker.prev->cores[i], &usageTracker.current->cores[i]);
        }

        usageTracker.current->total = CPU_stateBuffer.buffer[CPU_stateBuffer.head].total;

        pthread_mutex_unlock(&CPU_stateBuffer.access_mtx);

        sem_post(&CPU_stateBuffer.empty_sem);
    }
    sem_post(&CPU_stateBuffer.full_sem);
    pthread_exit(NULL);
}