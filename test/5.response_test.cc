#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <http/http_response.h>

int main() {
    using namespace http;
    std::string s = "HTTP/1.1 200 OK\r\n"
                    "server: ecstatic-3.3.2; ASP.NET\r\n"
                    "Content-Type: text/html\r\n"
                    "etag: W/\"12937205529-288-2020-07-19T09:31:18.981Z\"\r\n"
                    "last-modified: Sun, 19 Jul 2020 09:31:18 GMT\r\n"
                    "cache-control: max-age=3600\r\n"
                    "Date: Sun, 19 Jul 2020 09:32:10 GMT\r\n"
                    "Connection: keep-alive\r\n"
                    "Content-Length: 9\r\n"
                    "\r\n"
                    "{\"a\":\"b\"}";


    http_response resp;
    std::istringstream input;
    input.str(s);
    resp.read(input);
    assert(resp.body == "{\"a\":\"b\"}");
    assert(resp.headers.get("Content-Type") == "text/html");
    assert(resp.headers.get("Content-Length") == "9");
    assert(resp.proto == "HTTP/1.1");
    assert(resp.status == "200 OK");
    assert(resp.status_code == 200);
    assert(resp.content_length == std::atoi(resp.headers.get("Content-Length").c_str()));
    assert(resp.headers.values("server").size() == 2);
    std::cout << resp;
    std::cout << "response test passed!";
}