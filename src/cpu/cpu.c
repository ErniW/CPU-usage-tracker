#include "cpu.h"

extern int NUM_CORES;

void CPU_readUsage(CPU_state* state, FILE* data){
    char line[256];

    while(fgets(line, sizeof(line), data) != NULL && strncmp(line, "cpu", 3) == 0){

        CPU_core core = CPU_parseUsage(line);

        if(core.id == -1){
            state->total = core;
        }
        else{
            state->cores[core.id] = core;
        }

    }
}

CPU_core CPU_parseUsage(char* line){
    CPU_core core;
    char prefix[5];

    sscanf( line, 
            "%4s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
            prefix,
            &core.user, 
            &core.nice, 
            &core.system, 
            &core.idle, 
            &core.iowait, 
            &core.irq, 
            &core.softirq, 
            &core.steal, 
            &core.guest, 
            &core.guest_nice);

    if(prefix[3] == 0) 
        core.id = -1;
    else 
        sscanf(prefix, "cpu%lld", &core.id);

    return core;
}

void copy_CPU_state(CPU_state** n, CPU_state* s) {
    *n = (CPU_state*)malloc(sizeof(CPU_state));
    (*n)->cores = (CPU_core*)malloc((unsigned long)NUM_CORES * sizeof(CPU_core));

    for(int i=0; i<NUM_CORES; i++){
        (*n)->cores[i] = s->cores[i]; 
    }

    (*n)->total = s->total;
}

unsigned int CPU_getAverageUsage(CPU_core *prev, CPU_core *next){

    unsigned int cpuPercentage = 0;

    unsigned long prevIdle      = prev->idle + prev->iowait;
    unsigned long prevActive    = prev->user
                                + prev->nice
                                + prev->system
                                + prev->irq
                                + prev->softirq
                                + prev->steal;

    unsigned long currentIdle   = next->idle + next->iowait;
    unsigned long currentActive = next->user
                                + next->nice
                                + next->system
                                + next->irq
                                + next->softirq
                                + next->steal;

    unsigned long prevTotal     = prevIdle + prevActive;
    unsigned long currentTotal  = currentIdle + currentActive;

    unsigned long totalDiff     = currentTotal - prevTotal;
    unsigned long idleDiff      = currentIdle - prevIdle;

     
    if(totalDiff == 0) return 0;
    cpuPercentage  = (unsigned int)(totalDiff - idleDiff) * 100 / totalDiff;
    return cpuPercentage;
}
