#include "printer.h"
#include <queue.h>

extern volatile __sig_atomic_t exitFlag;
extern pthread_mutex_t access_mtx;
extern CPU_state state;
extern CPU_state prevState;
extern pthread_cond_t condition;
extern unsigned int usage;
extern Queue CPU_stateBuffer;


void* printerFunction(void* args){

    while(!exitFlag){

        pthread_mutex_lock(&CPU_stateBuffer.access_mtx);

        printf("%u\n", usage);
        prevState = state;
        
        pthread_mutex_unlock(&CPU_stateBuffer.access_mtx);
       
        sleep(1);
    }

    pthread_exit(NULL);
}