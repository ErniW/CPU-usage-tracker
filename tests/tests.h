#pragma once

#include <assert.h>

#define GLOBAL_CPU_MOCK     "cpu  8838 0 9158 19645149 88 0 946 0 0 0"
#define CORE_CPU_MOCK       "cpu2 757 0 721 2456218 1 0 10 0 0 0"

/*
    Previous Idle = 1200
    Previous active = 1200

    Current Idle = 2400
    Current active = 2400
*/

#define PREV_CPU_MOCK       "cpu0 200 200 200 1000 200 200 200 200 0 0"
#define CURRENT_CPU_MOCK    "cpu0 400 400 400 2000 400 400 400 400 0 0"

void should_return_total_cpu_id(void);
void should_return_correct_parsed_cpu_struct(void);

void should_compute_cpu_usage(void);
