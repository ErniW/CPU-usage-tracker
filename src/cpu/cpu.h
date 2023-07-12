#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void CPU_readUsage(CPU_state* state);
CPU_core CPU_parseUsage(char* line);

unsigned int CPU_getAverageUsage(CPU_core* prev, CPU_core* next);
