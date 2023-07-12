PROJECT_NAME = cpu_usage_tracker

CC := clang

SRCS = src/main.c

ifeq ($(CC), gcc)
	CFLAGS = -Wall -Wextra
else ifeq ($(CC), clang)
	CFLAGS = -Weverything 
endif

CFLAGS += -std=c11

compile:
	$(CC) $(SRCS) $(CFLAGS) -o build/prod/$(PROJECT_NAME).elf -pthread
	./build/prod/$(PROJECT_NAME).elf

test:
	$(CC) $(SRCS) $(CFLAGS) -o build/debug/$(PROJECT_NAME).elf -DDEBUG -pthread 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s build/debug/$(PROJECT_NAME).elf