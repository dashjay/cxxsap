//
// Created by 赵文杰 on 2020/7/22.
//

#ifndef HTTP_TEST_HTTP_RESPONSE_H
#define HTTP_TEST_HTTP_RESPONSE_H

#include <string>
#include <http/header.h>
#include <http/http_request.h>

namespace http {

    class http_response {
    public:
        std::string status; // "200 OK"
        int status_code; //200

        std::string proto; // "HTTP/1.1"
        header headers;

        std::string body;
        int64_t content_length;
        http_request *req;

        // 从输入流读取返回值
        void read(std::istream &input);

        std::string to_string();
    };
}


#endif //HTTP_TEST_HTTP_RESPONSE_H
