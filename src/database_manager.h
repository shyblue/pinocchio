#pragma once

#include <string>
#include <functional>
#include <boost/asio.hpp>
#include <tbb/tbb_thread.h>

#include <redis/redis_async_client.h>
#include <redis/redis_sync_client.h>
#include <thread>
#include "logger.h"

class TDatabaseManager {

public:
    explicit TDatabaseManager(const std::string& ip, const std::string& port);
    ~TDatabaseManager() { }

    int test();
    void run() {
        io_thread_ = std::thread([this]{
            io_service_.run();
            ST_LOGGER.Trace() << "DB IO thread Exiting.";
        }); }

    void stop() {
            io_service_.stop();
            io_thread_.join(); }

    RedisSyncClient* getSyncClient() { return &sync_client_; }
    RedisAsyncClient* getAsyncClient() { return &async_client_; }

    bool IsMember(const std::string& userToken);
    bool AddMember(const std::string& userToken);
    bool AddMsg(const std::string& userToken, const std::string& msg);
    bool GetMsg(const std::string& userToken, std::vector<RedisValue>& arr);
    void onConnected(bool ok, const std::string& err_msg);
private:
    int Initialize();

    const std::string ip_;
    const std::string port_;
    boost::asio::io_service io_service_;
    RedisAsyncClient async_client_;
    RedisSyncClient sync_client_;
    std::thread io_thread_;
};