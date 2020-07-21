//
// Created by 赵文杰 on 2020/7/19.
//

#include <map>
#include <vector>
#include <string>

#ifndef HTTP_H
#define HTTP_H


namespace http {
    using MIME = std::map<std::string, std::vector<std::string>>;

    class pesudo_mime;

    class header;

    class url_values;

    class http_request;

    namespace httputil {
        void parse_first_line(http_request &r, std::string line);

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

        static std::map<std::string, METHODS> methods_str{
                {"OPTIONS", METHODS::OPTIONS},
                {"GET",     METHODS::GET},
                {"HEAD",    METHODS::HEAD},
                {"POST",    METHODS::POST},
                {"PUT",     METHODS::PUT},
                {"DELETE",  METHODS::DELETE},
                {"TRACE",   METHODS::TRACE},
                {"CONNECT", METHODS::CONNECT},
        };

        bool valid_method(std::string &method);

        std::string trim_right(const std::string &input, char cutset);

        std::string trim_left(const std::string &input, char cutset);

// trim_space 去除字符串两端的空格
        std::string trim_space(const std::string &input);

        std::string trim_crlf(const std::string &input);

        std::vector<std::string> split(const std::string &input, char cutset);

        std::vector<std::string> split(const std::string &input, const std::string &cutset);
    }

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

    class http_request {
    private:
        std::string method;
        std::string url;

        std::string proto;
        int proto_major{};
        int proto_minor{};

        header hdr;

        std::string body;

        int64_t content_length{};

        bool close{};

        std::string host;

        url_values values;

        std::string remote_addr;
    public:
        http_request() = default;

        int read(std::istream &in);

        std::string &Method() { return method; }

        std::string &Url() { return url; }

        std::string &Proto() { return proto; }

        header &Headers() { return hdr; }

        int64_t &ContentLength() { return content_length; }

        std::string &Body() { return body; }

        url_values &UrlValues() { return values; }

        std::string to_string();
    };
}


#endif //HTTP_H
