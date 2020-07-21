#!/bin/zsh

set -x

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib/"

mkdir -p build &&
  cd build &&
  cmake .. &&
  make &&
  make test &&
  cd ..

mkdir -p build/bin/manual

g++ -std=c++11 \
    -o build/bin/manual/read_request_test \
    http/http.cc \
    test/manual/4.read_request_test.cc \
    -I. -lsockpp -lpthread

./build/bin/manual/read_request_test &

curl -q localhost:12345 -d '{"fix":"b"}' >>/dev/null 2>&1 &&
  curl -q localhost:12345\?a=b\&c=d >>/dev/null 2>&1
