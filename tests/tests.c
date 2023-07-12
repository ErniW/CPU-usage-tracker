#include <cpu.h>
#include "tests.h"

void should_return_global_cpu_id(void){

    CPU_core cpu = CPU_parseUsage(GLOBAL_CPU_MOCK);

    assert(cpu.id == -1);

    printf("TEST PASSED: should_return_global_cpu_id\n");
}

void should_return_correct_parsed_cpu_struct(void){

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

    printf("TEST PASSED: should_return_correct_parsed_cpu_struct\n");
}