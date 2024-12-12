#!/usr/bin/env bash

set -e
set -u
set -o pipefail

set -x

export CC=/opt/rocm/bin/amdclang
export CXX=/opt/rocm/bin/amdclang++

cmake \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    -DCMAKE_INSTALL_PREFIX=/opt/rocm \
    -DCMAKE_INSTALL_LIBDIR=lib \
    -B build

make -C build -j "$(nproc)"

set +x
echo -e "\n\n******************************************"
echo "Finished compilation!"
echo "Please run:"
echo "./build/main"
