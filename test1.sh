set -x

mkdir -p build/bin/manual

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib/"

g++ -std=c++17 \
  -o build/bin/manual/read_request_test \
  http/*.cc \
  test/manual/1.read_request_test.cc \
  -I. -lsockpp -lpthread

./build/bin/manual/read_request_test &

curl -q localhost:12345 -d '{"fix":"c"}' >>/dev/null 2>&1 &&
  curl -q localhost:12345\?a=b\&c=d >>/dev/null 2>&1

echo 0
