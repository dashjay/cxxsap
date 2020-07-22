//
// Created by 赵文杰 on 2020/7/22.
//

#ifndef HTTP_UNIT_H
#define HTTP_UNIT_H

#include <iostream>

namespace http {
    class http_unit {
        virtual void read(std::istream &input) = 0;

        virtual std::string to_string() = 0;

        friend std::ostream &operator<<(std::ostream &out, http_unit &unit) {
            out << unit.to_string();
            return out;
        }
    };
}

#endif // HTTP_UNIT_H
