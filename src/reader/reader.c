#include "reader.h"
#include <queue.h>

extern volatile __sig_atomic_t exitFlag;
extern CPU_usage usageTracker;
extern Queue CPU_stateBuffer;

void* readerFunction(void* args){

    while(!exitFlag){

        sem_wait(&CPU_stateBuffer.empty_sem);

        pthread_mutex_lock(&CPU_stateBuffer.access_mtx);

        CPU_readUsage(&CPU_stateBuffer.buffer[CPU_stateBuffer.tail]);
        
        CPU_stateBuffer.tail = (CPU_stateBuffer.tail + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&CPU_stateBuffer.access_mtx);

        sem_post(&CPU_stateBuffer.full_sem);
    }
    sem_post(&CPU_stateBuffer.full_sem);
    pthread_exit(NULL);
}