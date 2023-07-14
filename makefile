PROJECT_NAME = cpu_usage_tracker

CC := gcc

SRCS = src/main.c
SRCS += src/cpu/cpu.c
SRCS += src/reader/reader.c
SRCS += src/analyzer/analyzer.c
SRCS += src/printer/printer.c
SRCS += src/queue/queue.c
SRCS += src/utils/utils.c


INCLUDES = -Isrc/cpu/
INCLUDES += -Isrc/reader/
INCLUDES += -Isrc/analyzer/
INCLUDES += -Isrc/printer/
INCLUDES += -Isrc/queue/
INCLUDES += -Isrc/utils/

TESTS = tests/tests.c
TESTS += -Itests/

ifeq ($(CC), gcc)
	CFLAGS = -Wall -Wextra
else ifeq ($(CC), clang)
	CFLAGS = -Weverything 
endif

compile:
	$(CC) $(SRCS) $(CFLAGS) $(INCLUDES) -o build/prod/$(PROJECT_NAME).elf -pthread
	./build/prod/$(PROJECT_NAME).elf

test:
	$(CC) $(SRCS) $(TESTS) $(CFLAGS) $(INCLUDES) -o build/debug/$(PROJECT_NAME).elf -DDEBUG -pthread -g -O0
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s build/debug/$(PROJECT_NAME).elf