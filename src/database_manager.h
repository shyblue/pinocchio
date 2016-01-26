#pragma once

#include <string>
#include <functional>
#include <boost/asio.hpp>
#include <tbb/tbb_thread.h>

#include <redis/redis_async_client.h>

class TDatabaseManager {

public:
    explicit TDatabaseManager(const std::string& ip, const std::string& port);
    ~TDatabaseManager() {m_ioService.stop();}

    int test();
    size_t run() { return m_ioService.run(); }

    void onConnected(bool ok, const std::string& err_msg);
private:
    int Initialize();

    const std::string& m_ip;
    const std::string& m_port;
    boost::asio::io_service m_ioService;
    RedisAsyncClient m_client;
};