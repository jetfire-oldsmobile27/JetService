#pragma once
#include "logger.h"
#include "db.h"
#include "parser.h"
#include "restclient.h"

#include <boost/asio.hpp>

namespace jetfire27::Engine::Test {

    class TestServer {
    public:
        TestServer(unsigned short port, const std::string& dbPath);
        ~TestServer();
        void Run();
        void Stop();

    private:
        void HandleSession(boost::asio::ip::tcp::socket socket);

        unsigned short port_;
        jetfire27::Engine::DB::SQLiteDB db_;
        boost::asio::io_context ioc_;
        boost::asio::ip::tcp::acceptor acceptor_;
    };

}
