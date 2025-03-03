# This is a WIP C++ template repository

---

## How to setup dev environment

### Ubuntu

    sudo apt install cmake libfmt-dev

### Nix

    nix develop

---

### How to build

1. `cmake -B build`
2. `make -C build -j $(nproc)`

### How to run

- `./build/main`
