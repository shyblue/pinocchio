#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "config.h"
#include "logger.h"

Configure::Configure(void)
{
}

Configure::~Configure(void)
{
}

bool Configure::Initialize(const std::string config_file)
{
	try
	{
		LoadConfigureFile(config_file);
	}
	catch(std::exception& exception)
	{
		ST_LOGGER.Error() << "[ConfigureData] Initialize Error : " << exception.what();
		return false;
	}

	return true;
}

bool Configure::LoadConfigureFile( const std::string config_file)
{
	// 해당위치에서 Configure 파일을 읽는다.
	boost::filesystem::path path(boost::filesystem::initial_path());
	path /= config_file.c_str();
	if(!boost::filesystem::exists(boost::filesystem::path(path) ) )
	{
		// ERR_CONFIGURE_NOTPATH  Error 기록
		ST_LOGGER.Info() << "[ConfigureData][LoadCofigureFile] Nonexist File : " << path.string();
		return false;
	}

	m_configFile = config_file;
	boost::property_tree::ini_parser::read_ini(path.string().c_str(), m_iniTree);

	OutConfigureData();

	return true;
}

bool Configure::WriteConfigureFile()
{
	boost::property_tree::ini_parser::write_ini(m_configFile.c_str(), m_iniTree);
	return true;
}

void Configure::OutConfigureData()
{
	ST_LOGGER.Trace() << "[CONFIGURE DATA] [" << m_configFile << "]";
	for(boost::property_tree::ptree::value_type& v : m_iniTree)
	{
		std::string key = v.first;
		std::string value = v.second.get_value<std::string>();
		ST_LOGGER.Trace() << "    " << key << "=" << value;
	}
	ST_LOGGER.Trace() << "[END CONFIGURE DATA]";
}

void Configure::SetConfigureData(std::string key, uint32_t value)
{
	m_iniTree.erase(key);
	m_iniTree.put(key, value);
}

void Configure::SetConfigureData(std::string key, std::string value)
{
	m_iniTree.erase(key);
	m_iniTree.put(key, value);
}