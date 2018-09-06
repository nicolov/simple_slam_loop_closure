#!/usr/bin/env bash

set -euxo pipefail

rm -rf build
mkdir build
cd build
cmake ..
make -j$(nproc)
