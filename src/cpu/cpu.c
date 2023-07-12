#include "cpu.h"

void CPU_readUsage(CPU_state* state){
    FILE* data = fopen("/proc/stat", "r");
    if(data == NULL){
        perror("Error opening file");
        fclose(data);
        return;
    }

    char line[256];
    while(fgets(line, sizeof(line), data) != NULL && strncmp(line, "cpu", 3) == 0){
        // printf("%s", line);

        CPU_core core = CPU_parseUsage(line);

        if(core.id == -1){
            state->total = core;
        }
        else{
            state->cores[core.id] = core;
        }

        
        
    }

    fclose(data);
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

unsigned int CPU_getAverageUsage(CPU_core *prev, CPU_core *next){

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
    unsigned int cpuPercentage  = (totalDiff - idleDiff) * 100 / totalDiff;
    return cpuPercentage;

    // double cpuPercentage  = (double)(((double)totalDiff - (double)idleDiff) * 100 / (double)totalDiff);
    // return (unsigned int)cpuPercentage;
}
