#pragma once
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#define WATCHDOG_INTERVAL 2

enum watchdogID{
    READER_THREAD,
    ANALYZER_THREAD,
    PRINTER_THREAD
};

void watchdogUpdate(int id);
void* watchdog(void* args);
