//
// Created by 赵文杰 on 2020/7/22.
//

#ifndef HTTP_HTTPUTILS_H
#define HTTP_HTTPUTILS_H

#include <vector>
#include <string>
#include <map>

#include "http_request.h"
#include "http_response.h"

namespace http::httputils {
    void
    parse_request_line(http_request &r, std::string line);

    void
    parse_response_line(http_response &r, std::string line);

    enum METHODS {
        /*
     Method         = "OPTIONS"                ; Section 9.2
                    | "GET"                    ; Section 9.3
                    | "HEAD"                   ; Section 9.4
                    | "POST"                   ; Section 9.5
                    | "PUT"                    ; Section 9.6
                    | "DELETE"                 ; Section 9.7
                    | "TRACE"                  ; Section 9.8
                    | "CONNECT"                ; Section 9.9
        */
        OPTIONS = 0,
        GET,
        HEAD,
        POST,
        PUT,
        DELETE,
        TRACE,
        CONNECT,
        MAX_METHODS
    };

    static std::map<std::string, METHODS>
            methods_str{
            {"OPTIONS", METHODS::OPTIONS},
            {"GET",     METHODS::GET},
            {"HEAD",    METHODS::HEAD},
            {"POST",    METHODS::POST},
            {"PUT",     METHODS::PUT},
            {"DELETE",  METHODS::DELETE},
            {"TRACE",   METHODS::TRACE},
            {"CONNECT", METHODS::CONNECT},
    };

    bool
    valid_method(std::string &method);

    std::string
    trim_right(const std::string &input, char cutset);

    std::string
    trim_left(const std::string &input, char cutset);

// trim_space 去除字符串两端的空格
    std::string
    trim_space(const std::string &input);

    std::string
    trim_crlf(const std::string &input);

    std::vector<std::string>
    split(const std::string &input, char cutset);

    std::vector<std::string>
    split(const std::string &input, const std::string &cutset);

// check if it has multi value
    bool
    has_multi_value(const std::string &input);

    std::pair<std::string, std::string>
    split_pair(const std::string &input);

}
#endif // HTTP_HTTPUTILS_H
