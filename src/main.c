#include <cpu.h>
#include <reader.h>
#include <analyzer.h>
#include <printer.h>
#include <queue.h>
#include <utils.h>

#ifdef DEBUG
    #include <tests.h>
#endif

#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

pthread_t readerThread;
pthread_t analyzerThread;
pthread_t printerThread;

CPU_usage usageTracker;

Queue CPU_stateBuffer;

int NUM_CORES;

int main(){

    NUM_CORES = sysconf(_SC_NPROCESSORS_ONLN);

    usageTracker.prev = (CPU_state*)malloc(NUM_CORES * sizeof(CPU_state));
    usageTracker.current= (CPU_state*)malloc(NUM_CORES * sizeof(CPU_state));
    usageTracker.prev->cores = (CPU_core*)malloc(8 * sizeof(CPU_core));
    usageTracker.current->cores = (CPU_core*)malloc(8 * sizeof(CPU_core));
    usageTracker.coreValue = (unsigned int*)malloc(8 * sizeof(unsigned int));



    Queue_init(&CPU_stateBuffer);

    #ifdef DEBUG
        printf("\nStarting tests:\n\n");
        should_return_total_cpu_id();
        should_return_correct_parsed_core_struct();
        should_return_correct_parsed_cpu_state_struct();
        should_compute_cpu_usage();
        printf("\033[0;32mALL TESTS PASSED\033[0m\n");
    #endif

    signal(SIGINT, handleSIGINT);

    pthread_create(&readerThread, NULL, readerFunction, NULL);
    pthread_create(&analyzerThread, NULL, analyzerFunction, NULL);
    pthread_create(&printerThread, NULL, printerFunction, NULL);

    pthread_join(readerThread, NULL);
    pthread_join(analyzerThread, NULL);
    pthread_join(printerThread, NULL);
    

    for (int i = 0; i < BUFFER_SIZE; i++) {
        free(CPU_stateBuffer.buffer[i].cores);
    }

    return 0;
}
