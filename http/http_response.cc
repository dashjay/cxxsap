//
// Created by 赵文杰 on 2020/7/22.
//

#include "http_response.h"
#include <istream>
#include <string>
#include <http/httputils.h>
#include <http/errors.h>
#include <sstream>

void http::http_response::read(std::istream &input) {
    std::string line;
    std::getline(input, line);
    if (line.empty()) {
        throw errors::Error("http::http_response::read", "read first line error", "empty line");
    }

    try {
        httputil::parse_reesponse_line(*this, line);
    } catch (errors::Error &error) {
        std::cerr << error << '\n';
        throw;
    }
    auto i{this->status.find(' ')};
    auto code{this->status.substr(0, i)};
    if (code.length() != 3) {
        throw errors::Error("http::http_response::read", "malformed HTTP status code", status);
    }
    status_code = std::atoi(code.c_str());
    if (status_code < 0) {
        throw errors::Error("http::http_response::read", "malformed HTTP status code", status);
    }

    try {
        this->headers.read(input);
    } catch (errors::Error &error) {
        std::cerr << error << '\n';
        throw;
    }

    this->content_length = std::atoi(headers.get("Content-Length").c_str());
    input >> this->body;
    if (this->content_length != static_cast<int64_t>(this->body.length())) {
        std::cerr << "content length and body length mismatching";
    }
}

std::string http::http_response::to_string() {
    std::ostringstream str;
    str << proto << ' ' << status << "\r\n";
    str << headers.to_string();
    str << "\r\n";
    str << body;
    return str.str();
}
