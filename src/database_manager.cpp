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
        ST_LOGGER.Error("Can't connect to redis : %s:%s (%s)",m_ip.c_str(),m_port.c_str(),err_msg.c_str());
        return -1;
    }
    ST_LOGGER.Info("Redis Connected sync client : %s:%s", m_ip.c_str(),m_port.c_str());

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
        ST_LOGGER.Info("Redis Connected async client : %s:%s", m_ip.c_str(),m_port.c_str());
        test();
    }
    else
    {
        ST_LOGGER.Error("Can't connect to redis : %s:%s (%s)",m_ip.c_str(),m_port.c_str(),err_msg.c_str());
        m_ioService.stop();
    }
}

bool TDatabaseManager::IsMember(const std::string &userToken)
{

    auto result = m_syncClient.command("SISMEMBER","userToken",userToken);

    ST_LOGGER.Trace("IsMember [%s]",result.inspect().c_str());
    if(result.isOk())
    {
        if(result.toInt() == 1) return true;
    }

    ST_LOGGER.Error("SISMEMBER userToken %s result : %d",userToken.c_str(),result.toInt());
    return false;
}

bool TDatabaseManager::AddMember(const std::string &userToken)
{
    auto result = m_syncClient.command("SADD","userToken",userToken);

    if(result.isOk())
    {
        if(result.toInt() == 1) return true;
    }
    return false;
}

bool TDatabaseManager::AddMsg(const std::string &userToken, const std::string &msg)
{
    /*
    m_asyncClient.command("LPUSH",userToken,msg,[&](const RedisValue &v){
        ST_LOGGER.Trace("[Add message : %s]",v.toString().c_str());
    });
    */
    auto result = m_syncClient.command("LPUSH",userToken,msg);
    if(result.isOk())
    {
        if(result.toInt() == 1) return true;
    }
    return false;
}

bool TDatabaseManager::GetMsg(const std::string &userToken, std::string &msg)
{
    auto result = m_syncClient.command("RPOP",userToken);
    if(result.isOk())
    {
        msg = result.toString();
        return true;
    }
    return false;
}