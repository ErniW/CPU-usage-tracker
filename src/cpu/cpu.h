#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

struct CPU_core{
    long long id;
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long steal;
    unsigned long long guest;
    unsigned long long guest_nice;
};

typedef struct CPU_core CPU_core;

struct CPU_state{
    CPU_core total;
    CPU_core* cores;
};

typedef struct CPU_state CPU_state;

struct CPU_usage{
    CPU_state* prev;
    CPU_state* current;
    unsigned long total;
    unsigned int* value;
};

typedef struct CPU_usage CPU_usage;


void CPU_readUsage(CPU_state* state, FILE* data);
CPU_core CPU_parseUsage(char* line);
void copy_CPU_state(CPU_state** n, CPU_state* s);

unsigned int CPU_getAverageUsage(CPU_core* prev, CPU_core* next);
