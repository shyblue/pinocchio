#include <tbb/tbb_thread.h>
#include "database_manager.h"
#include "logger.h"

using namespace std::placeholders;

TDatabaseManager::TDatabaseManager(const std::string& ip,const std::string& port):
m_ip(ip),m_port(port),m_ioService(),m_client(m_ioService)
{
    Initialize();
}
int TDatabaseManager::Initialize()
{
    using tcp = boost::asio::ip::tcp;
    using address = boost::asio::ip::address;
    tcp::endpoint endpoint(address::from_string(m_ip),static_cast<u_short>(std::stoi(m_port)) );

    m_client.asyncConnect(endpoint,std::bind(&TDatabaseManager::onConnected,this,_1,_2));

    tbb::tbb_thread thread([&]{m_ioService.run();});
    return 0;
}

int TDatabaseManager::test()
{
    ST_LOGGER.Info("REDIS CLIENT DO TEST");
    const std::string key = "550e8400e29b41d4a71644665544000020151224110910";
    const std::string value = "Test Msg";

    m_client.command("SET", key, value, [&](const RedisValue &v)
    {
        ST_LOGGER.Info("SET: %s", v.toString().c_str());
    });

    m_client.command("GET", key, [&](const RedisValue &v)
    {
        ST_LOGGER.Info("GET: %s", v.toString().c_str());
    });

    m_client.command("DEL", key, [&](const RedisValue &v) {
        ST_LOGGER.Info("DEL Complete : %s", v.toString().c_str());
    });

    ST_LOGGER.Info("REDIS CLIENT TEST DONE");
    return 0;
}

void TDatabaseManager::onConnected(bool ok, const std::string &err_msg)
{
    if( ok )
    {
        ST_LOGGER.Info("Redis Connected : %s:%s", m_ip.c_str(),m_port.c_str());
        test();
    }
    else
    {
        ST_LOGGER.Error("Can't connect to redis : %s:%s (%s)",m_ip.c_str(),m_port.c_str(),err_msg.c_str());
        m_ioService.stop();
    }
}