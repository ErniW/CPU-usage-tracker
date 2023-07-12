#include "reader.h"

extern volatile __sig_atomic_t exitFlag;
extern pthread_mutex_t access_mtx;
extern CPU_state state;
extern pthread_cond_t condition;

void* readerFunction(void* args){

    while(!exitFlag){

        pthread_mutex_lock(&access_mtx);
        pthread_cond_signal(&condition);

        CPU_readUsage(&state);
         
        pthread_mutex_unlock(&access_mtx);
        usleep(100000);
    }

    pthread_exit(NULL);
}