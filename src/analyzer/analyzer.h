#pragma once

#include <cpu.h>
#include <pthread.h>

void* analyzerFunction(void* args);
void analyzerCleanup(void* args);
