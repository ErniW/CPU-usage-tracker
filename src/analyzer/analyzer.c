#include <analyzer.h>
#include <queue.h>

extern volatile __sig_atomic_t exitFlag;
extern pthread_mutex_t access_mtx;
extern CPU_state state;
extern CPU_state prevState;
extern pthread_cond_t condition;
extern unsigned int usage;
extern Queue CPU_stateBuffer;


#include <stdbool.h>

void* analyzerFunction(void* args){

    bool isInit = false;

    while(!exitFlag){

        sem_wait(&CPU_stateBuffer.full_sem);

        pthread_mutex_lock(&CPU_stateBuffer.access_mtx);
        CPU_stateBuffer.head = (CPU_stateBuffer.head + 1) % BUFFER_SIZE;

        if(!isInit){
            prevState = CPU_stateBuffer.buffer[CPU_stateBuffer.head];
            isInit = true; 
        }
        else{
            state = CPU_stateBuffer.buffer[CPU_stateBuffer.head];
            usage = CPU_getAverageUsage(&prevState.total, &state.total);
            

        }

        pthread_mutex_unlock(&CPU_stateBuffer.access_mtx);

        sem_post(&CPU_stateBuffer.empty_sem);
    }
    sem_post(&CPU_stateBuffer.full_sem);
    pthread_exit(NULL);
}