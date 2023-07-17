#include "reader.h"
#include <queue.h>
#include <watchdog.h>
#include <fcntl.h>

extern CPU_usage usage;
extern Queue buffer;

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

    pthread_mutex_unlock(&buffer.access_mtx);
    
    #ifdef DEBUG
        printf("Reader cleanup done\n");
    #endif
}

void* readerFunction(void* args){
    
    (void)args;
    
    pthread_cleanup_push(readerCleanup, data)

    while(1){
        
        watchdogUpdate(READER_THREAD);
  
        sem_wait(&buffer.empty_sem);
        pthread_mutex_lock(&buffer.access_mtx);

        data = fopen("/proc/stat", "r");

        Queue_push(&buffer, data);
        
        fclose(data);
        data = NULL;
        pthread_mutex_unlock(&buffer.access_mtx);
        sem_post(&buffer.full_sem);

    }

    pthread_cleanup_pop(1);
    pthread_exit(NULL);
}
