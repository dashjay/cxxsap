#include <iostream>
#include <http/http.h>
#include <string>
#include <cassert>

int main() {
    using namespace http;
    std::string a{"aaa;bbb;ccc"};
    auto res{http::httputil::split(a, ';')};
    assert(res[0] == "aaa");
    assert(res[1] == "bbb");
    assert(res[2] == "ccc");

    a = "aaaaaa";
    res = httputil::split(a, ';');
    assert(res[0] == a);

    a = "aaa&&bbb&&ccc";

    res = httputil::split(a, "&&");
    assert(res[0] == "aaa");
    assert(res[1] == "bbb");
    assert(res[2] == "ccc");

    a = "aaaaaa";
    res = httputil::split(a, '&&');
    assert(res[0] == "aaaaaa");
    std::cout << "split test passed!";
}