#pragma once

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include <string>
#include <list>

#include "redis_buffer.h"
#include "redis_value.h"
#include "redis_client_impl.h"

class RedisSyncClient : boost::noncopyable {
public:
     RedisSyncClient(boost::asio::io_service &ioService);
     ~RedisSyncClient();

    // Connect to redis server
     bool connect(
            const boost::asio::ip::tcp::endpoint &endpoint,
            std::string &errmsg);

    // Connect to redis server
     bool connect(
            const boost::asio::ip::address &address,
            unsigned short port,
            std::string &errmsg);

    // Set custom error handler. 
     void installErrorHandler(
        const boost::function<void(const std::string &)> &handler);

    // Execute command on Redis server.
     RedisValue command(const std::string &cmd);

    // Execute command on Redis server with one argument.
     RedisValue command(const std::string &cmd, const RedisBuffer &arg1);

    // Execute command on Redis server with two arguments.
     RedisValue command(
            const std::string &cmd, const RedisBuffer &arg1, const RedisBuffer &arg2);

    // Execute command on Redis server with three arguments.
     RedisValue command(
            const std::string &cmd, const RedisBuffer &arg1,
            const RedisBuffer &arg2, const RedisBuffer &arg3);

    // Execute command on Redis server with four arguments.
     RedisValue command(
            const std::string &cmd, const RedisBuffer &arg1, const RedisBuffer &arg2,
            const RedisBuffer &arg3, const RedisBuffer &arg4);

    // Execute command on Redis server with five arguments.
     RedisValue command(
            const std::string &cmd, const RedisBuffer &arg1,
            const RedisBuffer &arg2, const RedisBuffer &arg3,
            const RedisBuffer &arg4, const RedisBuffer &arg5);

    // Execute command on Redis server with six arguments.
     RedisValue command(
            const std::string &cmd, const RedisBuffer &arg1,
            const RedisBuffer &arg2, const RedisBuffer &arg3,
            const RedisBuffer &arg4, const RedisBuffer &arg5,
            const RedisBuffer &arg6);

    // Execute command on Redis server with seven arguments.
     RedisValue command(
            const std::string &cmd, const RedisBuffer &arg1,
            const RedisBuffer &arg2, const RedisBuffer &arg3,
            const RedisBuffer &arg4, const RedisBuffer &arg5,
            const RedisBuffer &arg6, const RedisBuffer &arg7);

    // Execute command on Redis server with the list of arguments.
     RedisValue command(
            const std::string &cmd, const std::list<std::string> &args);

protected:
     bool stateValid() const;

private:
    boost::shared_ptr<RedisClientImpl> pimpl;
};