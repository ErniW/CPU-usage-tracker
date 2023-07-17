#pragma once

#include <cpu.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 4

struct Queue{
    int head;
    int tail;
    sem_t empty_sem;
    sem_t full_sem;
    pthread_mutex_t access_mtx;
    CPU_state data[BUFFER_SIZE];
};

typedef struct Queue Queue;

/**
 * Initialize the queue and allocate its buffer. You can change buffer size 
 * in BUFFER_SIZE macro in queue.h
 * @param queue Queue object to initialize.
 */
void Queue_init(Queue* queue);

/**
 * Push data to buffer. 
 * @param queue Queue object to push.
 * @param data current state as raw data. Queue_push converts the value
 * and then places it into a buffer.
 */
void Queue_push(Queue* queue, FILE* data);

/**
 * Pop data from buffer.
 * @param queue Queue object to get value from.
 */
CPU_state Queue_pop(Queue* queue);
