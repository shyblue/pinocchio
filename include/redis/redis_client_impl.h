#pragma once

#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <string>
#include <vector>
#include <queue>
#include <map>

#include "redis/redis_parser.h"
#include "redis/redis_buffer.h"

class RedisClientImpl : public boost::enable_shared_from_this<RedisClientImpl> {
public:
     RedisClientImpl(boost::asio::io_service &ioService);
     ~RedisClientImpl();

     void handleAsyncConnect(
            const boost::system::error_code &ec,
            const boost::function<void(bool, const std::string &)> &handler);

     void close();

     static std::vector<char> makeCommand(const std::vector<RedisBuffer> &items);

     RedisValue doSyncCommand(const std::vector<RedisBuffer> &buff);

     void doAsyncCommand(
            const std::vector<char> &buff,
            const boost::function<void(const RedisValue &)> &handler);

     void sendNextCommand();
     void processMessage();
     void doProcessMessage(const RedisValue &v);
     void asyncWrite(const boost::system::error_code &ec, const size_t);
     void asyncRead(const boost::system::error_code &ec, const size_t);

     void onRedisError(const RedisValue &);
     void defaulErrorHandler(const std::string &s);
     static void ignoreErrorHandler(const std::string &s);

     static void append(std::vector<char> &vec, const RedisBuffer &buf);
     static void append(std::vector<char> &vec, const std::string &s);
     static void append(std::vector<char> &vec, const char *s);
     static void append(std::vector<char> &vec, char c);
    template<size_t size>
    static inline void append(std::vector<char> &vec, const char (&s)[size]);

    template<typename Handler>
    inline void post(const Handler &handler);

    enum {
        NotConnected,
        Connected,
        Subscribed,
        Closed 
    } state;

    boost::asio::strand strand;
    boost::asio::ip::tcp::socket socket;
    RedisParser redisParser;
    boost::array<char, 4096> buf;
    size_t subscribeSeq;

    typedef std::pair<size_t, boost::function<void(const std::vector<char> &buf)> > MsgHandlerType;
    typedef boost::function<void(const std::vector<char> &buf)> SingleShotHandlerType;

    typedef std::multimap<std::string, MsgHandlerType> MsgHandlersMap;
    typedef std::multimap<std::string, SingleShotHandlerType> SingleShotHandlersMap;

    std::queue<boost::function<void(const RedisValue &v)> > handlers;
    MsgHandlersMap msgHandlers;
    SingleShotHandlersMap singleShotMsgHandlers;

    struct QueueItem {
        boost::function<void(const RedisValue &)> handler;
        boost::shared_ptr<std::vector<char> > buff;
    };

    std::queue<QueueItem> queue;

    boost::function<void(const std::string &)> errorHandler;
};

template<size_t size>
void RedisClientImpl::append(std::vector<char> &vec, const char (&s)[size])
{
    vec.insert(vec.end(), s, s + size);
}

template<typename Handler>
inline void RedisClientImpl::post(const Handler &handler)
{
    strand.post(handler);
}