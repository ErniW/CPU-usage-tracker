mkdir ./build/release/
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build/release

mkdir ./build/debug/
cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build/debug