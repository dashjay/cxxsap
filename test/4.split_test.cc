#include <iostream>
#include <http/httputils.h>
#include <string>
#include <cassert>

int main() {
    using namespace http::httputils;

    std::string a{"aaa;bbb;ccc"};
    auto res{split(a, ';')};
    assert(res[0] == "aaa");
    assert(res[1] == "bbb");
    assert(res[2] == "ccc");

    a = "aaaaaa";
    res = split(a, ';');
    assert(res[0] == a);

    a = "aaa&&bbb&&ccc";

    res = split(a, "&&");
    assert(res[0] == "aaa");
    assert(res[1] == "bbb");
    assert(res[2] == "ccc");

    a = "aaaaaa";
    res = split(a, "&&");
    assert(res[0] == "aaaaaa");
    std::cout << "split test passed!";
}