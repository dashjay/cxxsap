#include <iostream>
#include <thread>
#include "sockpp/tcp_acceptor.h"
#include "sockpp/version.h"

static std::string ok = "HTTP/1.1 200 OK\r\n"
                        "server: ecstatic-3.3.2\r\n"
                        "Content-Type: text/html\r\n"
                        "etag: W/\"12937205529-288-2020-07-19T09:31:18.981Z\"\r\n"
                        "last-modified: Sun, 19 Jul 2020 09:31:18 GMT\r\n"
                        "cache-control: max-age=3600\r\n"
                        "Date: Sun, 19 Jul 2020 09:32:10 GMT\r\n"
                        "Connection: keep-alive\r\n"
                        "Content-Length: 9\r\n"
                        "\r\n"
                        "{\"a\":\"b\"}";

void run_echo(sockpp::tcp_socket sock) {
    ssize_t n;
    char buf[512];
    while ((n = sock.read(buf, sizeof(buf))) > 0) {
        sock.write(ok);
        break;
    }
}

int main(int argc, char *argv[]) {
    std::cout << "Sample TCP echo server for 'sockpp' "
              << sockpp::SOCKPP_VERSION << '\n' << std::endl;

    in_port_t port = (argc > 1) ? atoi(argv[1]) : 12345;

    sockpp::socket_initializer sockInit;

    sockpp::tcp_acceptor acc(port);

    if (!acc) {
        std::cerr << "Error creating the acceptor: " << acc.last_error_str() << std::endl;
        return 1;
    }
    //cout << "Acceptor bound to address: " << acc.address() << endl;
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
            std::thread thr(run_echo, std::move(sock));
            thr.detach();
        }
    }
}



