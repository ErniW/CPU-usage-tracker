#include "reader.h"
#include <queue.h>
#include <watchdog.h>

extern CPU_usage usageTracker;
extern Queue CPU_stateBuffer;

void readerCleanup(void* args) {
    printf("Reader cleanup called\n");

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

void* readerFunction(void* args){
    (void)args;

    pthread_cleanup_push(readerCleanup, NULL)

    while(1){

        watchdogUpdate(READER_THREAD);
  
        sem_wait(&CPU_stateBuffer.empty_sem);
        pthread_mutex_lock(&CPU_stateBuffer.access_mtx);

        Queue_push(&CPU_stateBuffer);

        pthread_mutex_unlock(&CPU_stateBuffer.access_mtx);
        sem_post(&CPU_stateBuffer.full_sem);

    }

    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}
