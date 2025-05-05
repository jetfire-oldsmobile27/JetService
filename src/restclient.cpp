#include "restclient.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/version.hpp>

using namespace boost;
using tcp = asio::ip::tcp;
namespace http = beast::http;
namespace json = boost::json;

jetfire27::Engine::REST::RestClient::RestClient(
    const std::string& base_url, 
    int port
) : base_url_(base_url), port_(port) {}

std::string jetfire27::Engine::REST::RestClient::build_full_url(const std::string& target) {
    return target;
}

template<class RequestBody>
std::string make_request(
    http::verb method,
    const std::string& base_url,
    int port,
    const std::string& target,
    const std::string& auth_token,
    RequestBody&& body
) {
    asio::io_context ioc;
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);

    auto const results = resolver.resolve(base_url, std::to_string(port));
    stream.connect(results);

    http::request<RequestBody> req{method, target, 11};
    req.set(http::field::host, base_url);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    
    if(!auth_token.empty()) {
        req.set(http::field::authorization, "Bearer " + auth_token);
    }

    if constexpr (!std::is_same_v<RequestBody, http::empty_body>) {
        req.set(http::field::content_type, "application/json");
    }

    req.body() = std::forward<RequestBody>(body);
    req.prepare_payload();

    http::write(stream, req);
    
    beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(stream, buffer, res);

    stream.socket().shutdown(tcp::socket::shutdown_both);

    if(res.result() != http::status::ok) {
        throw std::runtime_error(
            "HTTP error " + std::to_string(res.result_int()) + 
            ": " + res.reason().to_string()
        );
    }

    return res.body();
}

std::string jetfire27::Engine::REST::RestClient::Get(const std::string& target) {
    return make_request(
        http::verb::get,
        base_url_,
        port_,
        target,
        auth_token_,
        http::empty_body{}
    );
}

std::string jetfire27::Engine::REST::RestClient::Post(
    const std::string& target, 
    const json::value& body
) {
    return make_request(
        http::verb::post,
        base_url_,
        port_,
        target,
        auth_token_,
        json::serialize(body)
    );
}

std::string jetfire27::Engine::REST::RestClient::Put(
    const std::string& target, 
    const json::value& body
) {
    return make_request(
        http::verb::put,
        base_url_,
        port_,
        target,
        auth_token_,
        json::serialize(body)
    );
}

std::string jetfire27::Engine::REST::RestClient::Delete(const std::string& target) {
    return make_request(
        http::verb::delete_,
        base_url_,
        port_,
        target,
        auth_token_,
        http::empty_body{}
    );
}