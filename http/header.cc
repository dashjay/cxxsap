//
// Created by 赵文杰 on 2020/7/22.
//

#include "header.h"
#include <string>
#include <sstream>
#include <vector>
#include <http/httputils.h>

using namespace http;

void
pseudo_mime::add(const char *key, const std::string &value) {
    auto k{val.find(key)};
    if (k != val.end()) {
        k->second.push_back(value);
    } else {
        val[key] = std::vector<std::string>{value};
    }
}

void
pseudo_mime::set(const char *key, const std::string &value) {
    auto k{val.find(key)};
    if (k != val.end()) {
        k->second.clear();
        k->second.emplace_back(key);
    } else {
        val[key] = std::vector<std::string>{value};
    }
}

void
pseudo_mime::del(const char *key) {
    val.erase(key);
}

std::vector<std::string>
pseudo_mime::values(const char *key) {
    auto h{val.find(key)};
    if (h != val.end()) {
        return h->second;
    }
    return std::vector<std::string>();
}

MIME
pseudo_mime::clone() {
    return val;
}

std::ostream &
operator<<(std::ostream &out, pseudo_mime &m) {
    return out << m.to_string();
}

void
pseudo_mime::clear() {
    val.clear();
}

std::string
pseudo_mime::get(const char *key) {
    auto k{val.find(key)};
    if (k != val.end() && !k->second.empty()) {
        return k->second[0];
    }
    return std::string();
}

std::string
url_values::to_string() {
    std::ostringstream oss;
    auto start{val.cbegin()};
    while (start != val.cend()) {
        for (auto &j:start->second) {
            oss << start->first << "=" << j << "&";
        }
        start++;
    }
    auto res{oss.str()};
    return res.erase(res.length() - 1, 1);
}

std::pair<std::string, std::string>
split_pair(const std::string &input) {
    auto pos{input.find('=')};
    return std::make_pair(input.substr(0, pos), input.substr(pos + 1));
}

void
url_values::read(std::istream &input) {
    using namespace httputil;
    std::string line;
    if (std::getline(input, line)) {
        line = trim_left(line, '?');
        auto pos_begin{0};
        auto pos_divider{line.find('&')};
        while (pos_divider != std::string::npos) {
            auto single{line.substr(pos_begin, pos_divider - pos_begin)};
            if (single.find('=') != std::string::npos) {
                auto pair{split_pair(single)};
                this->add(pair.first.c_str(), pair.second);
            }
        }
        if (pos_begin != static_cast<int>(line.length())) {
            auto single{line.substr(pos_begin)};
            if (single.find('=') != std::string::npos) {
                auto pair{split_pair(single)};
                this->add(pair.first.c_str(), pair.second);
            }
        }
    }
}

void
url_values::parse_url(std::string input) {
    auto query{input.find('?')};
    if (query != std::string::npos) {
        input = input.substr(query + 1);
    }
    auto res = httputil::split(input, '&');
    if (res.empty()) {
        return;
    }
    for (auto &d: res) {
        if (d.find('=') != std::string::npos) {
            auto pair{split_pair(d)};
            this->add(pair.first.c_str(), pair.second);
        }
    }
}

std::string
header::to_string() {
    // 创建sstream当做buf
    std::ostringstream oss{};
    // 遍历
    auto k = val.cbegin();
    while (k != val.cend()) {
        // 存储key
        oss << k->first << ": ";
        // 如果value只有一个...
        if (k->second.size() == 1) {
            // ... 直接输入
            oss << k->second[0];
        } else {
            // 如果value 有多个
            for (auto &i:k->second) {
                oss << i;
                // 如果不是最后一个...
                if (i != k->second.back()) {
                    // ...输入; 来分割
                    oss << "; ";
                }
            }
        }
        // CRLF
        oss << "\r\n";
        k++;
    }
    return oss.str();
}

void
header::read(std::istream &input) {
    using namespace httputil;
    bool read_header{false};
    // line buffer
    std::string line;
    // getline
    while (std::getline(input, line)) {
        line = trim_right(line, '\r');
        // split_pair with :
        auto colon{line.find(':')};
        if (colon == std::string::npos) {
            break;
        }
        read_header = true;
        // get key
        std::string key{line.substr(0, colon)};

        // get value(s)
        auto value{trim_crlf(trim_space(line.substr(colon + 1)))};
        // trim \r\n
        value = trim_right(value, '\r');
        // check if ;...
        if (has_multi_value(value)) {
            // ... ; checked split_pair it into multi value
            for (auto &v: split(value, ';')) {
                add(key.c_str(), v);
            }
        } else {
            // ... ; not checked
            add(key.c_str(), value);
        }
    }
}


