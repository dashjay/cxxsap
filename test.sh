#!/bin/zsh

set -x

mkdir -p build \
  && cd build \
  && cmake .. \
  && make && make test \
  && cd ..
