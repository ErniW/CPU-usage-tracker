#pragma once

#include <assert.h>

#define GLOBAL_CPU_MOCK     "cpu  8838 0 9158 19645149 88 0 946 0 0 0"
#define CORE_CPU_MOCK       "cpu2 757 0 721 2456218 1 0 10 0 0 0"


void should_return_global_cpu_id(void);
void should_return_correct_parsed_cpu_struct(void);