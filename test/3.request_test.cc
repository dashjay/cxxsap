#include <iostream>
#include <http/http.h>
#include <sstream>
#include <cassert>


int main() {
    http_request r;
    std::stringstream input;
    input.str(
            "GET / HTTP/1.1\r\n"
            "X-Forwarded-For: 172.22.0.2\r\n"
            "Accept-Encoding: identity\r\n"
            "Content-Length: 9\r\n"
            "Conn-Id: f9b756f3-67bc-4e48-b76a-647a81c55ef9\r\n"
            "Unique-Id: 5163e05a-1e8e-4d29-ad31-8ba6ef67fb92\r\n"
            "User-Agent: Mozilla/5.0\r\n"
            "\r\n"
            "{\"a\":\"b\"}");
    if (r.read(input) != 0) {
        std::cout << "read fail" << "\n";
        return 1;
    }

    assert(r.Method() == "GET");
    assert(r.Url() == "/");
    assert(r.Proto() == "HTTP/1.1");

    auto hdr{r.Headers()};
    assert(hdr.get("X-Forwarded-For") == "172.22.0.2");
    assert(hdr.get("Accept-Encoding") == "identity");
    assert(hdr.get("Conn-Id") == "f9b756f3-67bc-4e48-b76a-647a81c55ef9");
    assert(hdr.get("Unique-Id") == "5163e05a-1e8e-4d29-ad31-8ba6ef67fb92");
    assert(hdr.get("User-Agent") == "Mozilla/5.0");

    assert(r.Body() == "{\"a\":\"b\"}");

    std::cout << "request test passed!" << '\n';
}