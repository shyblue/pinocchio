#pragma once

#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

class Config
{
public:
	Config(const std::string file_name, const std::string session_name);
	~Config();
public:
	boost::optional<std::string>	GetElementStr(std::string property_name);
	boost::optional<int>		GetElementInt(std::string property_name);
	boost::optional<double>		GetElementDouble(std::string property_name);

private:
	template <class Type>
	boost::optional<Type> GetElementType(const std::string& type_name);
	std::string MakePropertyName(const std::string& property_name);

	boost::property_tree::ptree m_pt;
	std::string m_sessionName;
};
