#include "watchdog.h"

extern pthread_t readerThread;
extern pthread_t analyzerThread;
extern pthread_t printerThread;
extern pthread_t watchdogThread;

extern pthread_mutex_t watchdog_mtx;
pthread_mutex_t watchdog_mtx;

extern int watchdogFlags[3];
int watchdogFlags[3] = {0};

void watchdogUpdate(int id){
    pthread_mutex_lock(&watchdog_mtx);
    watchdogFlags[id] = 1;
    pthread_mutex_unlock(&watchdog_mtx);
}

void* watchdog(void* args){
    (void)args;

    while(1){
        
        sleep(WATCHDOG_INTERVAL);
        pthread_mutex_lock(&watchdog_mtx);
        for(int i=0; i<3; i++){
            if(!watchdogFlags[i]){
                #ifdef DEBUG
                    printf("WATCHDOG thread [%d] not responding. Terminating...\n", i);
                #endif
                pthread_mutex_unlock(&watchdog_mtx);
                pthread_cancel(readerThread);
                pthread_cancel(analyzerThread);
                pthread_cancel(printerThread);
                pthread_exit(NULL);
            }

            watchdogFlags[i] = 0;
        }   
        pthread_mutex_unlock(&watchdog_mtx);
    }

}
