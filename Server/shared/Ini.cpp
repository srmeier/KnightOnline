#include "stdafx.h"
#include "Ini.h"
#include <iostream>
#include <fstream>
#include "tstring.h"

#define INI_BUFFER 512

CIni::CIni(const char *lpFilename)
{
	m_szFileName = lpFilename;
	Load(lpFilename);
}

bool CIni::Load(const char * lpFilename /*= nullptr*/)
{
	const char * fn = (lpFilename == nullptr ? m_szFileName.c_str() : lpFilename);
	std::ifstream file(fn);
	if (!file)
	{
		printf("Warning: %s does not exist, will use configured defaults.\n", fn);
		return false;
	}

	std::string currentSection;

	// If an invalid section is hit
	// Ensure that we don't place key/value pairs
	// from the invalid section into the previously loaded section.
	bool bSkipNextSection = false; 
	while (!file.eof())
	{
		std::string line;
		getline(file, line);

		rtrim(line);
		if (line.empty())
			continue;

		// Check for value strings first
		// It's faster than checking for a section
		// at the expense of of not being able to use '=' in section names.
		// As this is uncommon behaviour, this is a suitable trade-off.
		size_t keySeparatorPos = line.find(INI_KEY_SEPARATOR);
		if (keySeparatorPos != std::string::npos)
		{
			if (bSkipNextSection)
				continue;

			std::string key = line.substr(0, keySeparatorPos),
				value = line.substr(keySeparatorPos + 1);

			// Clean up key/value to allow for 'key = value'
			rtrim(key);   /* remove trailing whitespace from keys */
			ltrim(value); /* remove preleading whitespace from values */

			ConfigMap::iterator itr = m_configMap.find(currentSection);
			if (itr == m_configMap.end())
			{
				m_configMap.insert(std::make_pair(currentSection, ConfigEntryMap()));
				itr = m_configMap.find(currentSection);
			}

			itr->second[key] = value;
			continue;
		}

		// Not a value, so assume it's a section
		size_t sectionStart = line.find_first_of(INI_SECTION_START),
			sectionEnd = line.find_last_of(INI_SECTION_END);

		if (sectionStart == std::string::npos
			|| sectionEnd == std::string::npos
			|| sectionStart > sectionEnd)
		{
			/* invalid section */
			bSkipNextSection = true;
			continue;
		}

		currentSection = line.substr(sectionStart + 1, sectionEnd - 1);
		bSkipNextSection = false;
	}

	file.close();
	return true;
}

void CIni::Save(const char * lpFilename /*= nullptr*/)
{
	const char * fn = (lpFilename == nullptr ? m_szFileName.c_str() : lpFilename);
	FILE * fp = fopen(fn, "w");
	foreach (sectionItr, m_configMap)
	{
		// Start the section
		fprintf(fp, "[%s]" INI_NEWLINE, sectionItr->first.c_str());

		// Now list out all the key/value pairs
		foreach (keyItr, sectionItr->second)
			fprintf(fp, "%s=%s" INI_NEWLINE, keyItr->first.c_str(), keyItr->second.c_str());

		// Use a trailing newline to finish the section, to make it easier to read
		fprintf(fp, INI_NEWLINE);
	}
	fclose(fp);
}

int CIni::GetInt(const char* lpAppName, const char* lpKeyName, const int nDefault)
{
	ConfigMap::iterator sectionItr = m_configMap.find(lpAppName);
	if (sectionItr != m_configMap.end())
	{
		ConfigEntryMap::iterator keyItr = sectionItr->second.find(lpKeyName);
		if (keyItr != sectionItr->second.end())
			return atoi(keyItr->second.c_str());
	}

	SetInt(lpAppName, lpKeyName, nDefault);
	return nDefault;
}

bool CIni::GetBool(const char* lpAppName, const char* lpKeyName, const bool bDefault)
{
	return GetInt(lpAppName, lpKeyName, bDefault) == 1;
}

void CIni::GetString(const char* lpAppName, const char* lpKeyName, const char* lpDefault, std::string & lpOutString, bool bAllowEmptyStrings /*= true*/)
{
	ConfigMap::iterator sectionItr = m_configMap.find(lpAppName);
	if (sectionItr != m_configMap.end())
	{
		ConfigEntryMap::iterator keyItr = sectionItr->second.find(lpKeyName);
		if (keyItr != sectionItr->second.end())
		{
			lpOutString = keyItr->second;
			return;
		}
	}

	SetString(lpAppName, lpKeyName, lpDefault);
	lpOutString = lpDefault;
}

int CIni::SetInt(const char* lpAppName, const char* lpKeyName, const int nDefault)
{
	char tmpDefault[INI_BUFFER];
	_snprintf(tmpDefault, INI_BUFFER, "%d", nDefault);
	return SetString(lpAppName, lpKeyName, tmpDefault);
}

int CIni::SetString(const char* lpAppName, const char* lpKeyName, const char* lpDefault)
{
	ConfigMap::iterator itr = m_configMap.find(lpAppName);
	if (itr == m_configMap.end())
	{
		m_configMap.insert(std::make_pair(lpAppName, ConfigEntryMap()));
		itr = m_configMap.find(lpAppName);
	}
	itr->second[lpKeyName] = lpDefault;
	Save();
	return 1;
}
