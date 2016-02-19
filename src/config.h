#pragma once

#include <boost/serialization/singleton.hpp>
#include <boost/property_tree/ptree.hpp>

class Configure : public boost::serialization::singleton<Configure>
{
public:

	bool Initialize(const std::string ConfigureFilePath);
	bool WriteConfigureFile();

	// 사용할 설정 데이터를 얻어온다
	template<typename _T>
	const _T GetConfigureData(std::string key, const _T default_value);

	// 사용할 설정 데이터를 셋팅한다.
	void SetConfigureData(std::string key, uint32_t value);
	void SetConfigureData(std::string key, std::string value);

	void OutConfigureData();

protected:
	Configure(void);
	~Configure(void);
private:
	bool LoadConfigureFile(const std::string ConfigureFilePath);

	boost::property_tree::ptree			m_iniTree;
	std::string							m_configFile;
};

template<typename _T>
const _T Configure::GetConfigureData(std::string key, const _T default_value)
{
	return m_iniTree.get<_T>(key, default_value);
}

#define ST_CONFIG Configure::get_mutable_instance()
