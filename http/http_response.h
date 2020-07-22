//
// Created by 赵文杰 on 2020/7/22.
//

#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <istream>
#include <string>
#include "errors.h"
#include "header.h"
#include "http_unit.h"
#include "http_request.h"


namespace http {


    class http_response : public http_unit {
    public:
        std::string status; // "200 OK"
        int status_code; //200

        std::string proto; // "HTTP/1.1"
        header headers;

        std::string body;
        int64_t content_length;
        http_request *req;

        // 从输入流读取返回值
        void
        read(std::istream &input) override;

        std::string
        to_string() override;
    };

    bool read_response(http_response &resp, std::istream &input);
}

#endif // HTTP_RESPONSE_H
