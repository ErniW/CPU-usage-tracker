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
    CPU_state buffer[BUFFER_SIZE];
};

typedef struct Queue Queue;

void Queue_init(Queue* queue);
void Queue_push(Queue* queue, FILE* data);
CPU_state Queue_pop(Queue* queue);
