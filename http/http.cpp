//
// Created by 赵文杰 on 2020/7/19.
//

#include <iostream>
#include <sstream>
#include "http.h"

std::string trim_right(const std::string &input, const char cutset) {
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

std::string trim_left(const std::string &input, const char cutset) {
    auto start{0};
    while (input[start] == cutset) {
        start++;
    }
    return input.substr(start);
}

// trim_space 去除字符串两端的空格
std::string trim_space(const std::string &input) {
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

std::string trim_crlf(const std::string &input) {
    auto target{input.find("\r\n")};
    if (target == std::string::npos) {
        return input;
    } else {
        return input.substr(0, target);
    }
}

// check if it has multi value
bool has_multi_value(const std::string &input) {
    return input.find(';') != -1;
}

std::vector<std::string> split_multi_value(const std::string &input) {
    std::vector<std::string> res;
    auto pos{input.find(';')};
    int last = 0;
    while (pos != std::string::npos) {
        res.push_back(trim_space(input.substr(last, pos)));
        last = pos;
        pos = input.find(';', last + 1);
    }
    res.push_back(trim_space(input.substr(last + 1)));
    return res;
}


void pseudo_mime::add(const char *key, const std::string &value) {
    auto k{val.find(key)};
    if (k != val.end()) {
        k->second.push_back(value);
    } else {
        val[key] = std::vector<std::string>{value};
    }
}

void pseudo_mime::set(const char *key, const std::string &value) {
    auto k{val.find(key)};
    if (k != val.end()) {
        k->second.clear();
        k->second.emplace_back(key);
    } else {
        val[key] = std::vector<std::string>{value};
    }
}

void pseudo_mime::del(const char *key) {
    val.erase(key);
}

std::vector<std::string> pseudo_mime::values(const char *key) {
    auto h{val.find(key)};
    if (h != val.end()) {
        return h->second;
    }
    return std::vector<std::string>();
}

MIME pseudo_mime::clone() {
    return val;
}

std::ostream &operator<<(std::ostream &out, pseudo_mime &m) {
    return out << m.to_string();
}

void pseudo_mime::clear() {
    val.clear();
}

std::string pseudo_mime::get(const char *key) {
    auto k{val.find(key)};
    if (k != val.end() && !k->second.empty()) {
        return k->second[0];
    }
    return std::string();
}

std::string url_values::to_string() {
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

std::pair<std::string, std::string> split(const std::string &input) {
    auto pos{input.find('=')};
    return std::make_pair(input.substr(0, pos), input.substr(pos + 1));
}

int url_values::read(std::istream &input) {
    std::string line;
    if (std::getline(input, line)) {
        line = trim_left(line, '?');
        auto pos_begin{0};
        auto pos_divider{line.find('&')};
        while (pos_divider != std::string::npos) {
            auto single{line.substr(pos_begin, pos_divider - pos_begin)};
            if (single.find('=') != std::string::npos) {
                auto pair{split(single)};
                this->add(pair.first.c_str(), pair.second);
            }
        }
        if (pos_begin != line.length()) {
            auto single{line.substr(pos_begin)};
            if (single.find('=') != std::string::npos) {
                auto pair{split(single)};
                this->add(pair.first.c_str(), pair.second);
            }
        }
    }
    return 0;
}

std::string header::to_string() {
    // 创建sstream当做buf
    std::ostringstream oss{};
    // 遍历
    auto k = val.cbegin();
    while (k != val.cend()) {
        // 存储key
        oss << k->first << ":";
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

int header::read(std::istream &input) {
    bool read_header{false};
    // line buffer
    std::string line;
    // getline
    while (std::getline(input, line)) {
        line = trim_right(line, '\r');
        if (read_header && line.length() == 0) {
            return 0;
        }
        // split with :
        auto split{line.find(':')};
        if (split == std::string::npos) {
            continue;
        }
        read_header = true;
        // get key
        std::string key{line.substr(0, split)};

        // get value(s)
        auto value{trim_crlf(trim_space(line.substr(split + 1)))};
        // trim \r\n
        value = trim_right(value, '\r');
        // check if ;...
        if (has_multi_value(value)) {
            // ... ; checked split it into multi value
            for (auto &v:split_multi_value(value)) {
                add(key.c_str(), v);
            }
        } else {
            // ... ; not checked
            add(key.c_str(), value);
        }
    }
    return 0;
}

int http_request::read(std::istream &in) {
    in >> method;
    in >> url;
    in >> proto;
    if (hdr.read(in) == 1) {
        return 1;
    }
    in >> body;
    return 0;
}
