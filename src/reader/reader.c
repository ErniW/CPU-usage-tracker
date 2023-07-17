#include "reader.h"
#include <queue.h>
#include <watchdog.h>
#include <fcntl.h>

extern CPU_usage usageTracker;
extern Queue CPU_stateBuffer;

extern FILE* data;
FILE* data = NULL;

void readerCleanup(void* args) {
    FILE* file = (FILE*)args;

    if (file != NULL) {
        #ifdef DEBUG
            printf("File closed on cleanup.\n");
        #endif
        fclose(file);
    }

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
    
    #ifdef DEBUG
        printf("Reader cleanup done\n");
    #endif
}

void* readerFunction(void* args){
    
    (void)args;
    
    pthread_cleanup_push(readerCleanup, data)

    while(1){
        
        watchdogUpdate(READER_THREAD);
  
        sem_wait(&CPU_stateBuffer.empty_sem);
        pthread_mutex_lock(&CPU_stateBuffer.access_mtx);

        data = fopen("/proc/stat", "r");

        Queue_push(&CPU_stateBuffer, data);
        
        fclose(data);
        data = NULL;
        
        pthread_mutex_unlock(&CPU_stateBuffer.access_mtx);
        sem_post(&CPU_stateBuffer.full_sem);

    }

    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}
