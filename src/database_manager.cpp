#include <tbb/tbb_thread.h>
#include "database_manager.h"
#include "logger.h"

using namespace std::placeholders;

TDatabaseManager::TDatabaseManager(const std::string& ip,const std::string& port):
m_ip(ip),m_port(port),m_ioService(),m_asyncClient(m_ioService), m_syncClient(m_ioService)
{
    Initialize();
}
int TDatabaseManager::Initialize()
{
    using tcp = boost::asio::ip::tcp;
    using address = boost::asio::ip::address;
    tcp::endpoint endpoint(address::from_string(m_ip),static_cast<u_short>(std::stoi(m_port)) );

    m_asyncClient.asyncConnect(endpoint,std::bind(&TDatabaseManager::onConnected,this,_1,_2));

    std::string err_msg;
    if( !m_syncClient.connect(endpoint, err_msg))
    {
        ST_LOGGER.Error() << "Can't connect to redis : " << m_ip << ":" << m_port << "(" << err_msg << ")";
        return -1;
    }
    ST_LOGGER.Info() << "Redis Connected sync client : " << m_ip << ":" << m_port;

    tbb::tbb_thread thread( [&]{ m_ioService.run(); } );
    return 0;
}

int TDatabaseManager::test()
{
/*
    ST_LOGGER.Info("REDIS CLIENT DO TEST");
    const std::string key = "550e8400e29b41d4a71644665544000020151224110910";
    const std::string value = "Test Msg";

    m_asyncClient.command("SET", key, value, [&](const RedisValue &v)
    {
        ST_LOGGER.Info("SET: %s", v.toString().c_str());
    });

    m_asyncClient.command("GET", key, [&](const RedisValue &v)
    {
        ST_LOGGER.Info("GET: %s", v.toString().c_str());
    });

    m_asyncClient.command("DEL", key, [&](const RedisValue &v) {
        ST_LOGGER.Info("DEL Complete : %s", v.toString().c_str());
    });

    auto result = m_syncClient.command("SMEMBERS", "userToken");
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
        ST_LOGGER.Info() << "Redis Connected async client : " << m_ip << ":" << m_port;
        test();
    }
    else
    {
        ST_LOGGER.Error() << "Can't connect to redis : " << m_ip << ":" << m_port << "(" << err_msg << ")";
        m_ioService.stop();
    }
}

bool TDatabaseManager::IsMember(const std::string &userToken)
{

    auto result = m_syncClient.command("SISMEMBER","userToken",userToken);

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
    auto result = m_syncClient.command("SADD","userToken",userToken);

    return result.isOk();
}

bool TDatabaseManager::AddMsg(const std::string &userToken, const std::string &msg)
{
    /*
    m_asyncClient.command("RPUSH",userToken,msg,[&](const RedisValue &v){
        ST_LOGGER.Trace("[Add message : %s]",v.toString().c_str());
    });
    */
    m_asyncClient.command("LPUSH",userToken,msg,[&](const RedisValue& v) {
        if (v.isOk()) {
            if (v.toInt() > 99) {
                m_asyncClient.command("LTRIM", userToken, "0", "99",
                                      [](const RedisValue &v) {
                                          ST_LOGGER.Trace() << "[LTRIM : " << v.toString() << "]";
                                      });
            }
        }
        else
        {
            ST_LOGGER.Error() << "[Couldn't send message : " <<
        }
    });
    return true;
}

bool TDatabaseManager::GetMsg(const std::string &userToken, std::vector<RedisValue> &arr)
{
    auto result = m_syncClient.command("LRANGE",userToken,"0","-1");
    if(result.isOk())
    {
        arr = result.toArray();
        m_asyncClient.command("LTRIM",userToken,"100","-1",
                              [](const RedisValue& v){ ST_LOGGER.Trace() << "[LTRIM : " << v.toString() << "]"; });
        return true;
    }
    return false;
}
