//
// Created by 赵文杰 on 2020/7/19.
//

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "http_unit.h"
#include "header.h"


#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

namespace http {
    class http_request : public http_unit {
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

        void read(std::istream &in) override;

        url_values &Values() {
            if (!url_parsed) {
                values.parse_url(url);
            }
            return values;
        }

        std::string to_string() override;
    };

    bool read_request(http_request &r, std::istream &input);

}
#endif // HTTP_REQUEST_H
