#include <boost/format.hpp>
#include <iostream>
#include "config.h"

Config::Config(const std::string file_name, const std::string session_name)
	:	m_sessionName(session_name)
{
	boost::property_tree::ini_parser::read_ini(file_name, m_pt);
}

Config::~Config()
{
	if (!m_sessionName.empty())
	{
		m_sessionName.clear();
	}

	if (!m_pt.empty())
	{
		m_pt.clear();
	}
}

boost::optional<std::string> Config::GetElementStr(std::string property_name)
{
	return GetElementType<std::string>(property_name);
}

boost::optional<int> Config::GetElementInt(std::string property_name)
{
	return GetElementType<int>(property_name);
}

boost::optional<double> Config::GetElementDouble(std::string property_name)
{
	return GetElementType<double>(property_name);
}

template <class Type>
boost::optional<Type> Config::GetElementType(const std::string& type_name)
{
	if (m_pt.empty())
	{	return boost::optional<Type>();	}

	std::string property_full_name = MakePropertyName(type_name);
	Type result_value;
	
	try
	{
		result_value = m_pt.get<Type>(property_full_name);
	}
	catch (boost::property_tree::ptree_bad_path& e)
	{
		std::cout << "[exception] bad path : " << property_full_name << ", what : " << e.what() << std::endl;
		return boost::optional<Type>();
	}
	catch (boost::property_tree::ptree_bad_data& e)
	{
		std::cout << "[exception] bad data : " << property_full_name << ", what : " << e.what() << std::endl;
		return boost::optional<Type>();
	}

	return boost::optional<Type>(result_value);
}

std::string Config::MakePropertyName(const std::string& property_name)
{
	return boost::str(boost::format("%s.%s") % m_sessionName.c_str() % property_name.c_str());
}
