#include "queue.h"

void Queue_init(Queue* queue){
    queue->head = 0;
    queue->tail = 0;
    sem_init(&queue->empty_sem, 0, BUFFER_SIZE);
    sem_init(&queue->full_sem, 0, 0);
    pthread_mutex_init(&queue->access_mtx, NULL);

     for (int i = 0; i < BUFFER_SIZE; i++) {
        queue->buffer[i].cores = malloc(sizeof(CPU_core) * 8);
    }
}