#include <tbb/tbb_thread.h>
#include "database_manager.h"
#include "logger.h"

using namespace std::placeholders;

TDatabaseManager::TDatabaseManager(const std::string& ip,const std::string& port):
        ip_(ip), port_(port), io_service_(), async_client_(io_service_), sync_client_(io_service_)
{
    Initialize();
}
int TDatabaseManager::Initialize()
{
    using tcp = boost::asio::ip::tcp;
    using address = boost::asio::ip::address;
    tcp::endpoint endpoint(address::from_string(ip_), static_cast<u_short>(std::stoi(port_)) );

    async_client_.asyncConnect(endpoint, std::bind(&TDatabaseManager::onConnected, this, _1, _2));

    std::string err_msg;
    if( !sync_client_.connect(endpoint, err_msg))
    {
        ST_LOGGER.Error() << "Can't connect to redis : " << ip_ << ":" << port_ << "(" << err_msg << ")";
        return -1;
    }
    ST_LOGGER.Info() << "Redis Connected sync client : " << ip_ << ":" << port_;

    run();
    return 0;
}

int TDatabaseManager::test()
{
/*
    ST_LOGGER.Info("REDIS CLIENT DO TEST");
    const std::string key = "550e8400e29b41d4a71644665544000020151224110910";
    const std::string value = "Test Msg";

    async_client_.command("SET", key, value, [&](const RedisValue &v)
    {
        ST_LOGGER.Info("SET: %s", v.toString().c_str());
    });

    async_client_.command("GET", key, [&](const RedisValue &v)
    {
        ST_LOGGER.Info("GET: %s", v.toString().c_str());
    });

    async_client_.command("DEL", key, [&](const RedisValue &v) {
        ST_LOGGER.Info("DEL Complete : %s", v.toString().c_str());
    });

    auto result = sync_client_.command("SMEMBERS", "userToken");
    if(result.isOk())
    {
        auto x = result.toArray();
        for( const auto& v : x) {
            ST_LOGGER.Trace("   [userToken][%s]", v.toString().c_str());
        }
    }
    ST_LOGGER.Info("REDIS CLIENT TEST DONE");
*/
    return 0;
}

void TDatabaseManager::onConnected(bool ok, const std::string &err_msg)
{
    if( ok )
    {
        ST_LOGGER.Info() << "Redis Connected async client : " << ip_ << ":" << port_;
        test();
    }
    else
    {
        ST_LOGGER.Error() << "Can't connect to redis : " << ip_ << ":" << port_ << "(" << err_msg << ")";
        io_service_.stop();
    }
}

bool TDatabaseManager::IsMember(const std::string &userToken)
{

    auto result = sync_client_.command("SISMEMBER", "userToken", userToken);

    ST_LOGGER.Trace() << "IsMember [" << result.inspect() << "]";
    if(result.isOk())
    {
        if(result.toInt() == 1) return true;
    }

    ST_LOGGER.Error() <<  "SISMEMBER userToken " << userToken << " result : " << result.toInt();
    return false;
}

bool TDatabaseManager::AddMember(const std::string &userToken)
{
    auto result = sync_client_.command("SADD", "userToken", userToken);

    return result.isOk();
}

bool TDatabaseManager::AddMsg(const std::string &userToken, const std::string &msg)
{
    /*
    async_client_.command("RPUSH",userToken,msg,[&](const RedisValue &v){
        ST_LOGGER.Trace("[Add message : %s]",v.toString().c_str());
    });
    */
    async_client_.command("LPUSH", userToken, msg, [=](const RedisValue& v) {
        if (v.isOk()) {
            if (v.toInt() > 99) {
                async_client_.command("LTRIM", userToken.c_str(), "0", "99",
                                      [](const RedisValue &v) {
                                          ST_LOGGER.Trace() << "[LTRIM : " << v.toString() << "]";
                                      });
            }
        }
        else
        {
            ST_LOGGER.Error() << "[Couldn't send message : " << msg << " To : " << userToken ;
        }
    });
    return true;
}

bool TDatabaseManager::GetMsg(const std::string &userToken, std::vector<RedisValue> &arr)
{
    auto result = sync_client_.command("LRANGE", userToken, "0", "-1");
    if(result.isOk())
    {
        arr = result.toArray();
        async_client_.command("LTRIM", userToken, "100", "-1",
                              [](const RedisValue& v){ ST_LOGGER.Trace() << "[LTRIM : " << v.toString() << "]"; });
        return true;
    }
    return false;
}
