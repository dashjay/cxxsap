//
// Created by 赵文杰 on 2020/7/22.
//


#ifndef HTTP_ERRORS_H
#define HTTP_ERRORS_H

#include <exception>
#include <string>
#include <iostream>

namespace errors {
// http error class
    class Error : public std::exception {
    public:
        std::string where, what, detail;

        Error(std::string &&_where,
              std::string &&_what,
              std::string _detail) : where{std::move(_where)},
                                     what{std::move(_what)},
                                     detail{std::move((_detail))} {}

        friend std::ostream &operator<<(std::ostream &out, Error &error) {
            out << "[where: " + error.where + " ] [what: " + error.what + " ] [detail: " + error.detail + " ]";
            return out;
        }
    };


}


#endif // HTTP_ERRORS_H
