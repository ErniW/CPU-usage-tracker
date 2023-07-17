#include <cpu.h>
#include "tests.h"

extern int NUM_CORES;

void should_return_total_cpu_id(void){

    CPU_core cpu = CPU_parseUsage(GLOBAL_CPU_MOCK);

    assert(cpu.id == -1);

    printf("TEST PASSED: should_return_global_cpu_id\n");
}

void should_return_correct_parsed_core_struct(void){

    CPU_core cpu = CPU_parseUsage(CORE_CPU_MOCK);

    assert(cpu.id == 2);
    assert(cpu.user == 757);
    assert(cpu.nice == 0);
    assert(cpu.system == 721);
    assert(cpu.idle == 2456218);
    assert(cpu.iowait == 1);
    assert(cpu.irq == 0);
    assert(cpu.softirq == 10);
    assert(cpu.steal == 0);
    assert(cpu.guest == 0);
    assert(cpu.guest_nice == 0);

    printf("TEST PASSED: should_return_correct_parsed_core_struct\n");
}

void should_compute_cpu_usage(void){

    CPU_core prev = CPU_parseUsage(PREV_CPU_MOCK);
    CPU_core current = CPU_parseUsage(CURRENT_CPU_MOCK);

    unsigned int usage = CPU_getAverageUsage(&prev, &current);

    assert(usage == 50);

    printf("TEST PASSED: should_compute_cpu_usage\n");
}

void should_return_correct_parsed_cpu_state_struct(void){

    CPU_state state;
    FILE* data = fopen("/proc/stat", "r");

    state.cores = malloc(sizeof(CPU_core) * (unsigned long)NUM_CORES);
    
    CPU_readUsage(&state, data);

    assert(state.total.id == -1);
    for(int i=0; i<NUM_CORES; i++){
        assert(state.cores[i].id == i);
    }

    fclose(data);
    free(state.cores);

    printf("TEST PASSED: should_return_correct_parsed_cpu_state_struct\n");
}
