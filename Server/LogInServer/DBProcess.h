#pragma once

#include "../shared/database/OdbcConnection.h"

class CDBProcess  
{
public:
	bool Connect(std::string & szDSN, std::string & szUser, std::string & szPass);

	bool LoadVersionList();
	bool LoadUserCountList();

	uint16_t AccountLogin(std::string & strAccountID, std::string & strPasswd);
	int16_t AccountPremium(std::string & strAccountID);

private:
	OdbcConnection m_dbConnection;
};