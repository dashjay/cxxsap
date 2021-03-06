#include <cassert>
#include <http/httputils.h>
#include <iostream>

int main() {
    using namespace http::httputils;
    assert(trim_space("   aaa   ") == "aaa");
    assert(trim_crlf("aaa\r\n") == "aaa");
    assert(trim_right("aaa;;", ';') == "aaa");
    assert(trim_left(":aaa", ':') == "aaa");

    assert(trim_space("aaa") == "aaa");
    assert(trim_crlf("aaa") == "aaa");
    assert(trim_right("aaa", ';') == "aaa");
    assert(trim_left("aaa", ':') == "aaa");

    std::cout << "1.trim_test.cc passed" << "\n";
}