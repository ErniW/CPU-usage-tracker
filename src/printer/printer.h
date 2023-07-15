#pragma once

#include <cpu.h>
#include <pthread.h>
#include <unistd.h>

#define COLOR_INVERSED  "\x1b[7m"
#define COLOR_CLEAR     "\x1b[0m"
#define COLOR_GREEN     "\x1b[32m"
#define COLOR_YELLOW    "\x1b[33m"
#define COLOR_RED       "\x1b[31m"
#define CLEAR_SCREEN    "\033[H\033[J"

void* printerFunction(void* args);
void printCoreStats(int i, unsigned int usage);
