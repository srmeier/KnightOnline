#pragma once

#define	INI_SECTION_START	'['
#define	INI_SECTION_END		']'
#define	INI_KEY_SEPARATOR	'='
#define	INI_NEWLINE			"\n"

class CIni
{
private:
	std::string m_szFileName;
	// Defines key/value pairs within sections
	typedef std::map<std::string, std::string> ConfigEntryMap;

	// Defines the sections containing the key/value pairs
	typedef std::map<std::string, ConfigEntryMap> ConfigMap;

	ConfigMap m_configMap;

public:
	CIni(const char *lpFilename);

	bool Load(const char * lpFileName = nullptr);
	void Save(const char * lpFileName = nullptr);

	int GetInt(const char* lpAppName, const char* lpKeyName, const int nDefault);
	bool GetBool(const char* lpAppName, const char* lpKeyName, const bool bDefault);
	void GetString(const char* lpAppName, const char* lpKeyName, const char* lpDefault, std::string & lpOutString, bool bAllowEmptyStrings = true);
	int SetInt(const char* lpAppName, const char* lpKeyName, const int nDefault);
	int SetString(const char* lpAppName, const char* lpKeyName, const char* lpDefault);
};
