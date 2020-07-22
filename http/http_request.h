//
// Created by 赵文杰 on 2020/7/19.
//

#include <map>
#include <vector>
#include <string>
#include "header.h"

#ifndef HTTP_H
#define HTTP_H


namespace http {

    class http_request {
    private:
        url_values values;
        bool url_parsed{false};

    public:
        std::string method;
        std::string url;

        std::string proto;

        header headers;

        std::string body;

        int64_t content_length{};

        bool close{};

        std::string host;

        std::string remote_addr;
    public:
        http_request() = default;

        int read(std::istream &in);

        url_values &Values() {
            if (!url_parsed) {
                values.parse_url(url);
            }
            return values;
        }

        std::string to_string();
    };

    bool read_request(http_request &r, std::istream &input);
}


#endif //HTTP_H
