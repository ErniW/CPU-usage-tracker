#include "cpu.h"

void CPU_readUsage(){
    FILE* data = fopen("/proc/stat", "r");
    if(data == NULL){
        perror("Error opening file");
        fclose(data);
        return;
    }

    char line[256];
    while(fgets(line, sizeof(line), data) != NULL && strncmp(line, "cpu", 3) == 0){
        printf("%s", line);
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
