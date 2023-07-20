## CPU usage tracker

A simple terminal-based multithreaded app using POSIX to measure CPU usage from /proc/stat file written in c.

**The program consists of four threads:**
- Reader: reads the proc/stat file and parses it
- Analyzer: analyzes the CPU usage since last measurement interval which is 1 second.
- Printer: prints each second the total usage and usage per Core.
- Watchdog: to check when the app freezes.

### Build:
1. run `build_cmake.sh` to set up cmake builds for release and debug.
2. In root directory, run `make release` or `make debug`. *The debug includes some tests and runs valgrind.*

