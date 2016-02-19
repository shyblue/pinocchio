#include "user_manager.h"

int UserManager::Load()
{
    int nTotal=0;
    auto* p_client =m_spDbManager->getSyncClient();
    if(p_client)
    {
        ST_LOGGER.Trace() << "[LOAD userTokens START]";
        auto result = p_client->command("SMEMBERS","userToken");
        if(result.isOk())
        {

            auto x = result.toArray();
            for(const auto& v : x )
            {
                ++nTotal;
                ST_LOGGER.Trace()<< "    ["<< nTotal << "][" << v.toString() <<"]";
            }

        }
        ST_LOGGER.Trace() << "[LOAD userToken COMPLETE]";
    }

    return nTotal;
}