#include <analyzer.h>

extern volatile __sig_atomic_t exitFlag;
extern pthread_mutex_t access_mtx;
extern CPU_state state;
extern CPU_state prevState;
extern pthread_cond_t condition;

#include <stdbool.h>

void* analyzerFunction(void* args){

    bool isInit = false;

    while(!exitFlag){

        pthread_mutex_lock(&access_mtx);
        pthread_cond_wait(&condition, &access_mtx);
       
        if(!isInit) {
            prevState = state;
            isInit = true;
        }
        else{
             unsigned int usage = CPU_getAverageUsage(&prevState.total, &state.total);

            printf("%u\n", usage);
            prevState = state;
        }
        
         pthread_mutex_unlock(&access_mtx);
       
        sleep(2);
    }

    pthread_exit(NULL);
}