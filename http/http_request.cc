//
// Created by 赵文杰 on 2020/7/19.
//

#include <iostream>
#include <sstream>
#include <utility>
#include "http_request.h"
#include <http/httputils.h>
#include <http/errors.h>

using namespace http;


int http_request::read(std::istream &in) {
    std::string s;
    std::getline(in, s);

    try {
        httputil::parse_request_line(*this, s);
    } catch (errors::Error &error) {
        std::cerr << error << '\n';
        return 1;
    }

    try {
        headers.read(in);
    } catch (errors::Error &error) {
        std::cerr << error << '\n';
        return 1;
    }

    in >> body;
    content_length = std::atoi(headers.get("Content-Length").c_str());

    if (content_length != static_cast<int64_t>(body.length())) {
        std::cerr << "content length and body length mismatching";
    }

    return 0;
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


bool http::read_request(http_request &r, std::istream &input) {
    return r.read(input) == 0;
}
