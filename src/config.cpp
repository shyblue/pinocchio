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

bool Configure::LoadConfigureFile( const std::string config_file_path)
{
	// 해당위치에서 Configure 파일을 읽는다.
	boost::filesystem::path path(boost::filesystem::initial_path());
	path /= config_file_path.c_str();
	if(!boost::filesystem::exists(boost::filesystem::path(path) ) )
	{
		// ERR_CONFIGURE_NOTPATH  Error 기록
		ST_LOGGER.Info() << "[ConfigureData][LoadCofigureFile] Nonexist File : " << path.string();
		return false;
	}

	config_file_ = config_file_path;
	boost::property_tree::ini_parser::read_ini(path.string().c_str(), ini_tree_);

	OutConfigureData();

	return true;
}

bool Configure::WriteConfigureFile()
{
	boost::property_tree::ini_parser::write_ini(config_file_.c_str(), ini_tree_);
	return true;
}

void Configure::OutConfigureData()
{
	ST_LOGGER.Trace() << "[CONFIGURE DATA] [" << config_file_ << "]";
	for(boost::property_tree::ptree::value_type& v : ini_tree_)
	{
		std::string key = v.first;
		std::string value = v.second.get_value<std::string>();
		ST_LOGGER.Trace() << "    " << key << "=" << value;
	}
	ST_LOGGER.Trace() << "[END CONFIGURE DATA]";
}

void Configure::SetConfigureData(std::string key, uint32_t value)
{
	ini_tree_.erase(key);
	ini_tree_.put(key, value);
}

void Configure::SetConfigureData(std::string key, std::string value)
{
	ini_tree_.erase(key);
	ini_tree_.put(key, value);
}