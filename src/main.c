#include <stdio.h>
#include <tests.h>
#include <cpu.h>

int main(){


    #ifdef DEBUG
        printf("\nStarting tests:\n\n");
        should_return_total_cpu_id();
        should_return_correct_parsed_cpu_struct();
        should_compute_cpu_usage();
        printf("\033[0;32mALL TESTS PASSED\033[0m\n");
    #endif

    CPU_readUsage();

    return 0;
}
