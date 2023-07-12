
#include <cpu.h>
#include <reader.h>
#include <analyzer.h>
#include <printer.h>

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

pthread_mutex_t access_mtx = PTHREAD_MUTEX_INITIALIZER;
CPU_state state;
CPU_state prevState;
unsigned int usage = 0;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;


volatile __sig_atomic_t exitFlag = 0;

void handleSIGINT(int signal) {
    printf("[SIGINT %d] received. Exiting...\n", signal);
    exitFlag = 1;
 }

int main(){

    int coresAmount = sysconf(_SC_NPROCESSORS_ONLN);
    state.cores = malloc(sizeof(CPU_core) * coresAmount);


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

    free(state.cores);

    return 0;
}
