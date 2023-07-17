## CPU usage tracker

A simple terminal-based multithreaded app using POSIX to measure CPU usage from /proc/stat file written in c.

### Build

`make compile`

### Debug build
It will run tests, log some additional info and run valgrind to check for possible memory leaks. Tested on Ubuntu.

`make debug`
