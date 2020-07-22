#!/bin/zsh

set -ex

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib/"

mkdir -p build &&
  cd build &&
  cmake .. &&
  make &&
  make test &&
  cd ..
