#pragma once
#include <string>

namespace jetfire27::Engine::REST {
    class RestClient {
    public:
        std::string Get(const std::string& host, const std::string& target, int port = 80);
    };
}
