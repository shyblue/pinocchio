#pragma once

#include <string>
#include <functional>
#include <boost/asio.hpp>
#include <tbb/tbb_thread.h>

#include <redis/redis_async_client.h>
#include <redis/redis_sync_client.h>

class TDatabaseManager {

public:
    explicit TDatabaseManager(const std::string& ip, const std::string& port);
    ~TDatabaseManager() {m_ioService.stop();}

    int test();
    size_t run() { return m_ioService.run(); }

    RedisSyncClient* getSyncClient() { return &m_syncClient; }
    RedisAsyncClient* getAsyncClient() { return &m_asyncClient; }

    bool IsMember(const std::string& userToken);
    bool AddMember(const std::string& userToken);
    bool AddMsg(const std::string& userToken, const std::string& msg);
    bool GetMsg(const std::string& userToken, std::string& msg);
    void onConnected(bool ok, const std::string& err_msg);
private:
    int Initialize();

    const std::string& m_ip;
    const std::string& m_port;
    boost::asio::io_service m_ioService;
    RedisAsyncClient m_asyncClient;
    RedisSyncClient m_syncClient;
};