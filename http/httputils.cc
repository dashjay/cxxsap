//
// Created by 赵文杰 on 2020/7/22.
//

#include "httputils.h"
#include "errors.h"
#include "http_response.h"

namespace http::httputils {

// parse request first line
// like 'GET / HTTP/1.1\r\n'
    void
    parse_request_line(http_request &r, std::string line) {
        line = trim_right(line, '\r');
        auto idx1{line.find(' ')};
        if (idx1 == std::string::npos) {
            throw errors::Error("parse_request_line", "malformed HTTP request", line);
        }
        r.method = line.substr(0, idx1);
        std::string other{line.substr(idx1 + 1)};
        auto idx2{other.find(' ')};
        if (idx2 == std::string::npos) {
            throw errors::Error("parse_request_line", "malformed HTTP request", line);
        }
        r.url = other.substr(0, idx2);
        r.proto = other.substr(idx2 + 1);
    }

    void
    parse_response_line(http_response &r, std::string line) {
        line = trim_right(line, '\r');
        auto idx1{line.find(' ')};
        if (idx1 == std::string::npos) {
            throw errors::Error("parse_response_line", "malformed HTTP response", line);
        }
        r.proto = line.substr(0, idx1);
        r.status = trim_space(line.substr(idx1 + 1));
    }

// check method in list
    bool
    valid_method(std::string &method) {
        return !method.empty() && methods_str.find(method) != methods_str.end();
    }

// trim right
    std::string
    trim_right(const std::string &input, const char cutset) {
        if (input.empty()) {
            return input;
        }
        auto length{input.length()};
        while (length >= 0) {
            if (input[length - 1] == cutset) {
                length--;
            } else {
                break;
            }
        }
        return input.substr(0, length);
    }


// trim left
    std::string
    trim_left(const std::string &input, const char cutset) {
        auto start{0};
        while (input[start] == cutset) {
            start++;
        }
        return input.substr(start);
    }

// trim space on two side
    std::string
    trim_space(const std::string &input) {
        int start{0};
        int end{static_cast<int>(input.length() - 1)};
        while (input[start] == ' ') {
            start++;
        }
        while (input[end] == ' ') {
            end--;
        }
        return input.substr(start, end - start + 1);
    }

// trim \r\n crlf
    std::string
    trim_crlf(const std::string &input) {
        auto target{input.find("\r\n")};
        if (target == std::string::npos) {
            return input;
        } else {
            return input.substr(0, target);
        }
    }

    std::vector<std::string>
    split(const std::string &input, char cutset) {
        std::vector<std::string> res;
        auto pos{input.find(cutset)};
        int last = 0;
        while (pos != std::string::npos) {
            res.push_back(trim_space(input.substr(last, pos - last)));
            last = pos + 1;
            pos = input.find(cutset, last);
        }
        res.push_back(trim_space(input.substr(last)));
        return res;
    }

    std::vector<std::string>
    split(const std::string &input, const std::string &cutset) {
        std::vector<std::string> res;
        auto pos{input.find(cutset)};
        int last = 0;
        while (pos != std::string::npos) {
            res.push_back(trim_space(input.substr(last, pos - last)));
            last = pos + cutset.length();
            pos = input.find(cutset, last);
        }
        res.push_back(trim_space(input.substr(last)));
        return res;
    }

    bool
    has_multi_value(const std::string &input) {
        return input.find(';') != std::string::npos;
    }

    std::pair<std::string, std::string>
    split_pair(const std::string &input) {
        auto pos{input.find('=')};
        return std::make_pair(input.substr(0, pos), input.substr(pos + 1));
    }
}