//
// Created by 赵文杰 on 2020/7/22.
//
#include <string>
#include <map>
#include <vector>

#ifndef HTTP_TEST_HEADER_H
#define HTTP_TEST_HEADER_H
namespace http {
    using MIME = std::map<std::string, std::vector<std::string>>;

    class pseudo_mime {
    public:
        MIME val;

        std::string get(const char *key);

        void add(const char *key, const std::string &value);

        void set(const char *key, const std::string &value);

        void del(const char *key);

        std::vector<std::string> values(const char *key);

        MIME clone();

        virtual std::string to_string() = 0;

        friend std::ostream &operator<<(std::ostream &out, pseudo_mime &m);

        virtual void read(std::istream &input) = 0;

        void clear();
    };

    class header : public pseudo_mime {
    public:
        std::string to_string() override;

        void read(std::istream &input) override;
    };

    class url_values : public pseudo_mime {
    public:
        std::string to_string() override;

        void read(std::istream &input) override;

        void parse_url(std::string input);
    };
}


#endif //HTTP_TEST_HEADER_H
