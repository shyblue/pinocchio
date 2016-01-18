#include <vector>
#include <map>
#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>

template<typename _key, typename _value>
class MapContainer
{
public:
	typedef boost::unordered_map<_key, _value> CacheContainer;
	MapContainer(void) {};
	~MapContainer(void) {};

	bool Insert(_key& key, _value value);
	_value* GetValue(_key key);
	std::vector<_value*> AllValue();

	CacheContainer GetContainer();
protected:


private:
	_value* Find(_key& key);
	CacheContainer m_cacheContainer;
};

template<typename _key, typename _value>
_value* MapContainer<_key, _value>::Find(_key& key)
{
	typename CacheContainer::iterator iter = m_cacheContainer.find(key);
	if (iter != m_cacheContainer.end())
	{
		return &iter->second;
	}

	return 0;	
}

template<typename _key, typename _value>
bool MapContainer<_key, _value>::Insert(_key& key, _value value)
{
	// 컨테이너에 데이터 존재하지 않으면 데이터 삽입한다.
	_value* p = Find(key);
	if (!p)
	{
		m_cacheContainer.insert(typename CacheContainer::value_type(key, value));	
		return true;
	}

	return false;
}

template<typename _key, typename _value>
_value* MapContainer<_key, _value>::GetValue(_key key)
{
	// 컨테이너에 데이터가 존재하면 데이터를 리턴해준다.
	_value* p = Find(key);
	if (p)
	{
		return p;
	}

	return 0;
}

template<typename _key, typename _value>
typename MapContainer<_key, _value>::CacheContainer MapContainer<_key, _value>::GetContainer()
{
	return m_cacheContainer;
}

template<typename _key, typename _value>
std::vector<_value*> MapContainer<_key, _value>::AllValue()
{
	std::vector<_value*> result;
	BOOST_FOREACH(typename CacheContainer::value_type value, m_cacheContainer)
	{
		result.push_back(&value.second);
	}

	return result;
}