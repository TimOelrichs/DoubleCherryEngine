#pragma once

#include <string>
#include <memory>
#include <httplib.h>

class HTTPClient {
public:
    static HTTPClientManager& instance() {
        static HTTPClientManager instance;
        return instance;
    }

    // Einfacher GET-Request
    std::string get(const std::string& host, const std::string& path) {
        httplib::Client cli(host.c_str());
        if (auto res = cli.Get(path.c_str())) {
            if (res->status == 200) {
                return res->body;
            }
        }
        return {};
    }

    // Einfacher POST-Request mit body (z. B. JSON)
    std::string post(const std::string& host, const std::string& path, const std::string& body, const std::string& content_type = "application/json") {
        httplib::Client cli(host.c_str());
        if (auto res = cli.Post(path.c_str(), body, content_type.c_str())) {
            if (res->status == 200) {
                return res->body;
            }
        }
        return {};
    }

private:
    HTTPClientManager() = default;
    ~HTTPClientManager() = default;
    HTTPClientManager(const HTTPClientManager&) = delete;
    HTTPClientManager& operator=(const HTTPClientManager&) = delete;
};
