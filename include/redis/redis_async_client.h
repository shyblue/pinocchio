#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/noncopyable.hpp>

#include <functional>
#include <string>
#include <list>

#include "redis_value.h"
#include "redis_buffer.h"
#include "redis_client_impl.h"

using namespace std::placeholders;
class RedisAsyncClient : boost::noncopyable {
public:
    // Subscribe handle.
    struct Handle {
        size_t id;
        std::string channel;
    };

     RedisAsyncClient(boost::asio::io_service &ioService);
     ~RedisAsyncClient();

    // Connect to redis server
     void connect(
            const boost::asio::ip::address &address,
            unsigned short port,
            const std::function<void(bool, const std::string &)> &handler);

    // Connect to redis server
     void connect(
            const boost::asio::ip::tcp::endpoint &endpoint,
            const std::function<void(bool, const std::string &)> &handler);

    // backward compatibility
    inline void asyncConnect(
            const boost::asio::ip::address &address,
            unsigned short port,
            const std::function<void(bool, const std::string &)> &handler)
    {
        connect(address, port, handler);
    }

    // backward compatibility
    inline void asyncConnect(
            const boost::asio::ip::tcp::endpoint &endpoint,
            const std::function<void(bool, const std::string &)> &handler)
    {
        connect(endpoint, handler);
    }


    // Set custom error handler. 
     void installErrorHandler(
        const std::function<void(const std::string &)> &handler);

    // Execute command on Redis server.
     void command(
            const std::string &cmd,
            const std::function<void(const RedisValue &)> &handler = &dummyHandler);

    // Execute command on Redis server with one argument.
     void command(
            const std::string &cmd, const RedisBuffer &arg1,
            const std::function<void(const RedisValue &)> &handler = &dummyHandler);

    // Execute command on Redis server with two arguments.
     void command(
            const std::string &cmd, const RedisBuffer &arg1, const RedisBuffer &arg2,
            const std::function<void(const RedisValue &)> &handler = &dummyHandler);

    // Execute command on Redis server with three arguments.
     void command(
            const std::string &cmd, const RedisBuffer &arg1,
            const RedisBuffer &arg2, const RedisBuffer &arg3,
            const std::function<void(const RedisValue &)> &handler = &dummyHandler);

    // Execute command on Redis server with four arguments.
     void command(
            const std::string &cmd, const RedisBuffer &arg1, const RedisBuffer &arg2,
            const RedisBuffer &arg3, const RedisBuffer &arg4,
            const std::function<void(const RedisValue &)> &handler = &dummyHandler);

    // Execute command on Redis server with five arguments.
     void command(
            const std::string &cmd, const RedisBuffer &arg1,
            const RedisBuffer &arg2, const RedisBuffer &arg3,
            const RedisBuffer &arg4, const RedisBuffer &arg5,
            const std::function<void(const RedisValue &)> &handler = &dummyHandler);

    // Execute command on Redis server with six arguments.
     void command(
            const std::string &cmd, const RedisBuffer &arg1,
            const RedisBuffer &arg2, const RedisBuffer &arg3,
            const RedisBuffer &arg4, const RedisBuffer &arg5,
            const RedisBuffer &arg6,
            const std::function<void(const RedisValue &)> &handler = &dummyHandler);


    // Execute command on Redis server with seven arguments.
     void command(
            const std::string &cmd, const RedisBuffer &arg1,
            const RedisBuffer &arg2, const RedisBuffer &arg3,
            const RedisBuffer &arg4, const RedisBuffer &arg5,
            const RedisBuffer &arg6, const RedisBuffer &arg7,
            const std::function<void(const RedisValue &)> &handler = &dummyHandler);

    // Execute command on Redis server with the list of arguments.
     void command(
            const std::string &cmd, const std::list<RedisBuffer> &args,
            const std::function<void(const RedisValue &)> &handler = &dummyHandler);

    // Subscribe to channel. Handler msgHandler will be called
    // when someone publish message on channel. Call unsubscribe 
    // to stop the subscription.
     Handle subscribe(
            const std::string &channelName,
            const std::function<void(const std::vector<char> &msg)> &msgHandler,
            const std::function<void(const RedisValue &)> &handler = &dummyHandler);

    // Unsubscribe
     void unsubscribe(const Handle &handle);

    // Subscribe to channel. Handler msgHandler will be called
    // when someone publish message on channel; it will be 
    // unsubscribed after call.
     void singleShotSubscribe(
            const std::string &channel,
            const std::function<void(const std::vector<char> &msg)> &msgHandler,
            const std::function<void(const RedisValue &)> &handler = &dummyHandler);

    // Publish message on channel.
     void publish(
            const std::string &channel, const RedisBuffer &msg,
            const std::function<void(const RedisValue &)> &handler = &dummyHandler);

     static void dummyHandler(const RedisValue &) {}

protected:
     bool stateValid() const;

private:
    boost::shared_ptr<RedisClientImpl> pimpl;
};
