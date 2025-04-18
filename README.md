# This is a cross-platform C++ project with fmt::fmt built in

How? It uses ZIG to compile everything.

### How to compile
1. `cmake -B build`
2. `make -C build -j $(nproc)`

### How to compile without zig
1. `cmake -B build -DBUILD_WITH_ZIG=OFF`
2. `make -C build -j $(nproc)`

### How to run
- `./build/main`  
You should be able to produce a platform independent binary file.  
Try to copy it to another Linux machine and run it.
