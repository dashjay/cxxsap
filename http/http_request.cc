//
// Created by 赵文杰 on 2020/7/19.
//


#include "http_request.h"
#include "httputils.h"

namespace http {
    void http_request::read(std::istream &in) {
        std::string s;
        std::getline(in, s);

        try {
            httputils::parse_request_line(*this, s);
        } catch (errors::Error &error) {
            std::cerr << error << '\n';
            throw;
        }

        try {
            headers.read(in);
        } catch (errors::Error &error) {
            std::cerr << error << '\n';
            throw;
        }

        if (!httputils::valid_method(method)) {
            throw errors::Error("http_request::read", "error method", method);
        }

        in >> body;
        content_length = std::atoi(headers.get("Content-Length").c_str());

        if (content_length != static_cast<int64_t>(body.length())) {
            std::cerr << "content length and body length mismatching";
        }
    }

    std::string http_request::to_string() {
        std::ostringstream str;
        // GET / HTTP/1.1\r\n
        str << method << " " << url << " " << proto << "\r\n";
        // a: b\r\n
        // c: d; e\r\n
        str << headers.to_string();
        // \r\n between header & body
        str << "\r\n";
        // {"a":"b"} without \r\n
        str << body;
        return str.str();
    }


    bool read_request(http_request &r, std::istream &input) {
        try {
            r.read(input);
        } catch (errors::Error &error) {
            std::cerr << error << '\n';
            return false;
        }
        return true;
    }
}