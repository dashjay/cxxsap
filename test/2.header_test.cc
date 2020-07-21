#include "http/http.h"
#include <iostream>
#include <cassert>

int main() {
    header h;
    h.add("Content-Type", "application/json");
    h.add("server", "ASP.NET");
    h.add("X-Forwarded-For", "127.0.0.1");
    h.add("X-Forwarded-For", "0.0.0.0");
    h.set("last-modified", "Sun, 19 Jul 2020 09:31:18 GMT");

    url_values v;
    v.add("a", "b");
    v.add("c", "d");
    v.add("e", "f");
    v.add("g", "h");
    v.set("j", "k");


    assert(v.to_string() == "a=b&c=d&e=f&g=h&j=k");

    assert(h.to_string() == "Content-Type:application/json\r\n"
                            "X-Forwarded-For:127.0.0.1; 0.0.0.0\r\n"
                            "last-modified:Sun, 19 Jul 2020 09:31:18 GMT\r\n"
                            "server:ASP.NET\r\n");
    assert(v.get("a") == "b");

    assert(h.get("X-Forwarded-For") == "127.0.0.1");

    std::cout << "mime header test passed!";
    return 0;
}