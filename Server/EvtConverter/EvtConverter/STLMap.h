#ifndef __STLMAP_H_
#define __STLMAP_H_

#include <map>

using namespace std;

//-----------------------------------------------------------------------------
template <class T> class CSTLMap {
public:
	map<int, T*> m_UserTypeMap;

public:
	typedef typename map<int, T*>::iterator Iterator;
	typedef typename map<int, T*>::value_type ValueType;

public:
	int GetSize(void) { return m_UserTypeMap.size(); }

	bool PutData(int key_value, T* pData) {
		if(!pData) return false;

		pair<Iterator, bool> temp_pair;
		temp_pair = m_UserTypeMap.insert(ValueType(key_value, pData));

		return temp_pair.second;
	}

	T* GetData(int key_value) {
		T* pData = NULL;

		Iterator iter = m_UserTypeMap.find(key_value);

		if(iter == m_UserTypeMap.end()) return NULL;
		else pData = (*iter).second;

		return pData;
	}

	Iterator DeleteData(int key_value) {
		if(m_UserTypeMap.empty()) return m_UserTypeMap.end();

		Iterator iter = m_UserTypeMap.find(key_value);

		if(iter == m_UserTypeMap.end()) return m_UserTypeMap.end();
		else {
			T* pData = NULL;
			pData = (*iter).second;

			iter = m_UserTypeMap.erase(iter);

			delete pData;
			pData = NULL;

			return iter;
		}
	}

	void DeleteAllData(void) {
		Iterator iter1 = m_UserTypeMap.begin(), iter2 = m_UserTypeMap.end();

		for(; iter1 != iter2; ++iter1) {
			delete (*iter1).second;
		}

		m_UserTypeMap.clear();
	}

	bool IsExist(int key_value) {
		Iterator iter = m_UserTypeMap.find(key_value);

		if(iter == m_UserTypeMap.end()) return false;
		else return true;
	}

	bool IsEmpty(void) { return m_UserTypeMap.empty(); }

public:
	CSTLMap(void) {}

	virtual ~CSTLMap(void) {
		/*DeleteAllData();*/
	}
};

#endif
