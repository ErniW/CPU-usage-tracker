PROJECT_NAME = cpu_usage_tracker

CC := clang

SRCS = src/main.c
SRCS += src/cpu/cpu.c

INCLUDES += -Isrc/cpu/

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
	$(CC) $(SRCS) $(TESTS) $(CFLAGS) $(INCLUDES) -o build/debug/$(PROJECT_NAME).elf -DDEBUG -pthread 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s build/debug/$(PROJECT_NAME).elf