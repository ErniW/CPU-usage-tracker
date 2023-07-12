
#include <cpu.h>
#include <reader.h>

#ifdef DEBUG
    #include <tests.h>
#endif

#include <stdio.h>
#include <pthread.h>
#include <signal.h>

pthread_t readerThread;

volatile __sig_atomic_t exitFlag = 0;

void handleSIGINT(int signal) {
    printf("[SIGINT %d] received. Exiting...\n", signal);
    exitFlag = 1;
 }

int main(){

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

    pthread_join(readerThread, NULL);
    //CPU_readUsage();

    return 0;
}
