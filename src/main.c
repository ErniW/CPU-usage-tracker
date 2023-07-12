#include <stdio.h>

int main(){


    #ifdef DEBUG
        printf("\nStarting tests:\n\n");
        should_return_global_cpu_id();
        should_return_correct_parsed_cpu_struct();
        printf("\033[0;32mALL TESTS PASSED\033[0m\n");
    #endif

    CPU_readUsage();

    return 0;
}
