#pragma once

#include "../shared/database/OdbcConnection.h"

class CDBProcess  
{
public:
	void UseShortFormVersionTable() {
		m_bUseShortFormVersionTable = true;
	}

	CDBProcess();
	bool Connect(const std::string& szDSN, const std::string& szUser, const std::string& szPass);

	bool LoadVersionList(bool bSuppressErrors = false);
	bool LoadUserCountList();
	bool LoadAccountMap();

	uint16_t AccountLogin(const std::string& strAccountID, const std::string& strPasswd) const;
	bool IsAccountLoggedIn(const std::string& strAccountID, uint16_t* sServerPortNo, std::string* szServerIP);
	int16_t AccountPremium(const std::string& strAccountID);

	size_t GetRegisteredUserCount() const {
		return m_accountMap.size();
	}

private:
	OdbcConnection	m_dbConnection;
	bool			m_bUseShortFormVersionTable;

	struct _TB_USER
	{
		std::string	strAccountID;
		std::string	strPasswd;
		uint8_t		byAuthority;
	};

	std::map<std::string, _TB_USER>	m_accountMap;
	mutable std::recursive_mutex	m_accountMapLock;
};