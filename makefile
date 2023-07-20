MAKEFLAGS += --no-print-directory

release:
	cmake --build ./build/release --target release

debug:
	cmake --build ./build/debug --target debug
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./build/debug/bin/cpu_usage_tracker_debug