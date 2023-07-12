#include "reader.h"

extern volatile __sig_atomic_t exitFlag;
extern pthread_mutex_t access_mtx;
extern CPU_state state;
extern pthread_cond_t condition;

void* readerFunction(void* args){

    while(!exitFlag){

        pthread_mutex_lock(&access_mtx);
        pthread_cond_signal(&condition);
        // CPU_state state;
        // int numCores = 8;
        

        CPU_readUsage(&state);

        // printf("%ld %llu\n", state.total.id, state.total.idle ); // should be -1
        // free(state.cores);
         
        pthread_mutex_unlock(&access_mtx);
        usleep(100000);
    }

    pthread_exit(NULL);
}