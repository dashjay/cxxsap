#include <iostream>
#include "http/http_request.h"
#include <thread>
#include <sstream>
#include "sockpp/tcp_acceptor.h"
#include "sockpp/version.h"
#include <cassert>

using namespace http;

void handler_request(sockpp::tcp_socket sock) {
    using namespace http;
    http_request r;
    char buf[4096];
    sock.read(buf, sizeof(buf));
    std::stringstream input;
    input.str(buf);
    r.read(input);

    std::cout << "Connection closed from " << sock.peer_address() << std::endl;
    if (r.url == "/") {
        assert(r.body == "{\"fix\":\"b\"}");
        assert(std::atoi(r.headers.get("Content-Length").c_str()) == r.body.length());
        std::cout << "read request passed" << '\n';
    } else {
        assert(r.url_values().to_string() == "a=b&c=d");
        exit(0);
    }

}

int main() {

    sockpp::socket_initializer sockInit;
    auto port{12345};
    sockpp::tcp_acceptor acc(12345);

    if (!acc) {
        std::cerr << "Error creating the acceptor: " << acc.last_error_str() << std::endl;
        return 1;
    }
    std::cout << "Acceptor bound to address: " << acc.address() << std::endl;
    std::cout << "Awaiting connections on port " << port << "..." << std::endl;
    while (true) {
        sockpp::inet_address peer;

        // Accept a new client connection
        sockpp::tcp_socket sock = acc.accept(&peer);
        std::cout << "Received a connection request from " << peer << std::endl;

        if (!sock) {
            std::cerr << "Error accepting incoming connection: "
                      << acc.last_error_str() << std::endl;
        } else {
            // Create a thread and transfer the new stream to it.
            std::thread thr(handler_request, std::move(sock));
            thr.detach();
        }
    }
}