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

extern CPU_usage usage;
extern Queue buffer;
extern pthread_t readerThread;
extern pthread_t analyzerThread;
extern pthread_t printerThread;
extern pthread_t watchdogThread;
extern int NUM_CORES;
extern FILE* data;

extern pthread_mutex_t watchdog_mtx;

pthread_t readerThread;
pthread_t analyzerThread;
pthread_t printerThread;
pthread_t watchdogThread;

CPU_usage usage;
Queue buffer;

int NUM_CORES;

int main(){

    NUM_CORES = (int)sysconf(_SC_NPROCESSORS_ONLN);

    usage.prev = NULL;
    usage.current = NULL;
    usage.value = (unsigned int*)malloc((unsigned long)NUM_CORES * sizeof(unsigned int));

    Queue_init(&buffer);

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
        free(buffer.data[i].cores);
    }

    if (data != NULL) {
        #ifdef DEBUG
            printf("File closed at exit.\n");
        #endif
        fclose(data);
    }

    return 0;
}
