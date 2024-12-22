#pragma once

#include "../shared/database/OdbcConnection.h"

class CDBProcess  
{
public:
	bool Connect(const std::string& szDSN, const std::string& szUser, const std::string& szPass);

	bool LoadVersionList();
	bool LoadUserCountList();

	uint16_t AccountLogin(const std::string& strAccountID, const std::string& strPasswd);
	bool IsAccountLoggedIn(const std::string& strAccountID, uint16_t* sServerPortNo, std::string* szServerIP);
	int16_t AccountPremium(const std::string& strAccountID);

private:
	OdbcConnection m_dbConnection;
};