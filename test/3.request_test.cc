#include <iostream>
#include <http/http_request.h>
#include <sstream>
#include <http/httputils.h>
#include <cassert>


int main() {
    using namespace http;
    std::string str = "GET / HTTP/1.1\r\n"
                      "X-Forwarded-For: 172.22.0.2\r\n"
                      "Accept-Encoding: identity\r\n"
                      "Content-Length: 9\r\n"
                      "Conn-Id: f9b756f3-67bc-4e48-b76a-647a81c55ef9; f1b756f3-67bc-4e48-b76a-647a81c55ef9\r\n"
                      "Unique-Id: 5163e05a-1e8e-4d29-ad31-8ba6ef67fb92\r\n"
                      "User-Agent: Mozilla/5.0\r\n"
                      "\r\n"
                      "{\"a\":\"b\"}";


    http_request r;
    std::stringstream input;
    input.str(str);
    if (!read_request(r, input)) {
        return -1;
    }

    assert(r.method == "GET");
    assert(r.url == "/");
    assert(r.proto == "HTTP/1.1");
    assert(r.headers.get("X-Forwarded-For") == "172.22.0.2");
    assert(r.headers.get("Accept-Encoding") == "identity");
    assert(r.headers.get("Unique-Id") == "5163e05a-1e8e-4d29-ad31-8ba6ef67fb92");
    assert(r.headers.get("User-Agent") == "Mozilla/5.0");

    assert(r.body == "{\"a\":\"b\"}");

    std::cout << r << '\n';
    for (auto &v :r.headers.values("Conn-Id")) {
        std::cout << v << '\n';
    }
    std::cout << "request test passed!" << '\n';
}