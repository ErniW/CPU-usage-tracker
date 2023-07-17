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

/**
 * Initialize CPU_usage struct.
 *
 * @param usage CPU_usage struct to initialize.
 */
void CPU_usage_init(CPU_usage* usage);

/**
 * Read current usage from raw proc/stat data.
 *
 * @param state CPU_state struct to store parsed data, usually a position in buffer.
 * @param data Recently read proc/stat data.
 */
void CPU_readUsage(CPU_state* state, FILE* data);

/**
 * Parse a single line from proc/stat. Used inside CPU_readUsage function.
 *
 * @param line CPU_state struct to store parsed data, usually a position in buffer.
 * @return parsed CPU_core usage stats.
 */
CPU_core CPU_parseUsage(char* line);

/**
 * Creates a new copy of state. A utility function to store current value
 * Remember to free the value.
 * @param copy new CPU_state to allocate
 * @param src source.
 */
void copy_CPU_state(CPU_state** copy, CPU_state* src);

/**
 * Get average core usage by comparing current value with last measure.
 * @param prev Previous core state
 * @param current Current core state.
 * @return % of usage
 */
unsigned int CPU_getAverageUsage(CPU_core* prev, CPU_core* current);
