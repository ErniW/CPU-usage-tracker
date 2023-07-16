#include <cpu.h>
#include <reader.h>
#include <analyzer.h>
#include <printer.h>
#include <queue.h>
#include <sigint.h>
#include <watchdog.h>

#ifdef DEBUG
    #include <tests.h>
#endif

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

extern CPU_usage usageTracker;
extern Queue CPU_stateBuffer;
extern pthread_t readerThread;
extern pthread_t analyzerThread;
extern pthread_t printerThread;
extern pthread_t watchdogThread;
extern int NUM_CORES;

extern pthread_mutex_t watchdog_mtx;

pthread_t readerThread;
pthread_t analyzerThread;
pthread_t printerThread;
pthread_t watchdogThread;

CPU_usage usageTracker;
Queue CPU_stateBuffer;

int NUM_CORES;

int main(){

    NUM_CORES = (int)sysconf(_SC_NPROCESSORS_ONLN);

    usageTracker.prev = NULL;
    usageTracker.current = NULL;
    usageTracker.coreValue = (unsigned int*)malloc((unsigned long)NUM_CORES * sizeof(unsigned int));

    Queue_init(&CPU_stateBuffer);

    pthread_mutex_init(&watchdog_mtx, NULL);

    #ifdef DEBUG
        printf("\nStarting tests:\n\n");
        should_return_total_cpu_id();
        should_return_correct_parsed_core_struct();
        should_return_correct_parsed_cpu_state_struct();
        should_compute_cpu_usage();
        printf("\033[0;32mALL TESTS PASSED\033[0m\n");
    #endif

    signal(SIGINT, SIGINTHandler);

    pthread_create(&readerThread, NULL, readerFunction, NULL);
    pthread_create(&analyzerThread, NULL, analyzerFunction, NULL);
    pthread_create(&printerThread, NULL, printerFunction, NULL);
    pthread_create(&watchdogThread, NULL, watchdog, NULL);



    pthread_join(readerThread, NULL);
    pthread_join(analyzerThread, NULL);
    pthread_join(printerThread, NULL);
    pthread_join(watchdogThread, NULL);
    

    for (int i = 0; i < BUFFER_SIZE; i++) {
        free(CPU_stateBuffer.buffer[i].cores);
    }

    // if(usageTracker.prev!= NULL) free(usageTracker.prev->cores);
    // if(usageTracker.current!= NULL) free(usageTracker.current->cores);
    // free(usageTracker.current);
    // free(usageTracker.prev);
    free(usageTracker.coreValue);
    return 0;
}
