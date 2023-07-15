#pragma once

#include <cpu.h>
#include <pthread.h>
#include <semaphore.h>

void* readerFunction(void* args);
