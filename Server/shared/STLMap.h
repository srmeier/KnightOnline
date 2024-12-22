#pragma once

#include <map>

template <
	class T>
class CSTLMap  
{
public:
	using MapType = std::map<uint32_t, T*>;
	MapType m_UserTypeMap;

	mutable std::recursive_mutex m_lock;

	// TODO: This should simply return size_t, but since this is used by everything
	// I won't fix this for now until I can go over it.
	int GetSize() const
	{
		return (int) m_UserTypeMap.size(); 
	}

	std::recursive_mutex& GetLock()
	{
		return m_lock;
	}

	bool IsExist(
		uint32_t key)
		const
	{
		Guard lock(m_lock);
		return (m_UserTypeMap.find(key) != m_UserTypeMap.end()); 
	}

	bool IsEmpty() const
	{ 
		return m_UserTypeMap.empty(); 
	}

	bool PutData(
		uint32_t key_value,
		T* pData) 
	{
		Guard lock(m_lock);
		return m_UserTypeMap.insert(std::make_pair(key_value, pData)).second; 
	}

	T* GetData(
		uint32_t key_value)
	{
		Guard lock(m_lock);
		auto itr = m_UserTypeMap.find(key_value);
		return (itr != m_UserTypeMap.end() ? itr->second : nullptr);
	}

	const T* GetData(
		uint32_t key_value)
		const
	{
		Guard lock(m_lock);
		auto itr = m_UserTypeMap.find(key_value);
		return (itr != m_UserTypeMap.end() ? itr->second : nullptr);
	}

	void DeleteData(
		uint32_t key_value)
	{
		Guard lock(m_lock);
		auto itr = m_UserTypeMap.find(key_value);
		if (itr!= m_UserTypeMap.end())
		{
			delete itr->second;
			m_UserTypeMap.erase(itr);
		}
	}

	void DeleteAllData()
	{
		Guard lock(m_lock);
		if (m_UserTypeMap.empty())
			return;

		for (auto itr = m_UserTypeMap.begin(); itr != m_UserTypeMap.end(); ++itr)
			delete itr->second;

		m_UserTypeMap.clear();
	}

	~CSTLMap()
	{
		DeleteAllData();
	}

	typename MapType::const_iterator begin() const
	{
		return m_UserTypeMap.begin();
	}

	typename MapType::const_iterator end() const
	{
		return m_UserTypeMap.end();
	}

	typename MapType::iterator begin()
	{
		return m_UserTypeMap.begin();
	}

	typename MapType::iterator end()
	{
		return m_UserTypeMap.end();
	}

	typename MapType::const_reverse_iterator rbegin() const
	{
		return m_UserTypeMap.rbegin();
	}

	typename MapType::const_reverse_iterator rend() const
	{
		return m_UserTypeMap.rend();
	}

	typename MapType::reverse_iterator rbegin()
	{
		return m_UserTypeMap.rbegin();
	}

	typename MapType::reverse_iterator rend()
	{
		return m_UserTypeMap.rend();
	}
};