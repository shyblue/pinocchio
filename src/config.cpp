#include "config.h"
#include "spdlog/spdlog.h"

#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>

ConfigureData::ConfigureData(void)
{
}

ConfigureData::~ConfigureData(void)
{
}

bool ConfigureData::Initialize(const std::string config_file)
{
	try
	{
		LoadCofigureFile(config_file);
	}
	catch(std::exception& exception)
	{
		//LOG_ERROR("[ConfigureData] Initialize Error[%s]", exception.what());
		return false;
	}

	return true;
}

bool ConfigureData::LoadConfigureFile( const std::string config_file)
{
	// 해당위치에서 Configure 파일을 읽는다.
	boost::filesystem::path path(boost::filesystem::initial_path());
	path /= config_file.c_str();
	if(!boost::filesystem::exists(boost::filesystem::path(path) ) )
	{
		// ERR_CONFIGURE_NOTPATH  Error 기록
		//LOG_ERROR("[ConfigureData][LoadCofigureFile] Nonexist File[%s]", path.c_str() );
		return false;
	}

	m_configFile = ConfigureFilePath;
	boost::property_tree::ini_parser::read_ini(path.string().c_str(), m_initTree);

	OutConfigureData();

	return true;
}

bool ConfigureData::WriteConfigureFile()
{
	boost::property_tree::ini_parser::write_ini(configure_path_.c_str(), m_initTree);
	return true;
}

void ConfigureData::OutConfigureData()
{
	auto log = spdlog::stdout_logger_mt("console");

	log->info("[OUT CONFIGURE DATA]");
	for(const auto& value : m_initTree)
	{
		std::string key = value.first;
		for(const auto& detail_value : value.second)
		{
			log->info("[%s] %s %s",key.c_str(),detail_value.first.c_str(), detail_value.second.data().c_str());
		}
	}
}

void ConfigureData::SetConfigureData(std::string key, uint32_t value)
{
	m_initTree.erase(key);
	m_initTree.put(key, value);
}

void ConfigureData::SetConfigureData(std::string key, std::string value)
{
	m_initTree.erase(key);
	m_initTree.put(key, value);
}

