#include "reader.h"

extern volatile __sig_atomic_t exitFlag;

void* readerFunction(void* args){

    while(!exitFlag){

        CPU_state state;
        int numCores = 8;

        state.cores = malloc(sizeof(CPU_core) * numCores);

        CPU_readUsage(&state);

        printf("%ld %llu\n", state.total.id, state.total.idle ); // should be -1
        free(state.cores);
        usleep(1000);
    }

    pthread_exit(NULL);
}