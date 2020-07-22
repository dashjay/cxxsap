#!/bin/zsh

set -ex
rm -rf build/*

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib/"

mkdir -p build &&
  cd build &&
  cmake .. &&
  make &&
  make test &&
  cd ..
