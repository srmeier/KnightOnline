#include "StdAfx.h"
#include "TblEditorBase.h"
#include <vector>
#include <map>

std::map<int, std::vector<std::string>> CTblEditorBase::Data;
int CTblEditorBase::memory_iDataTypeCount = 0;
enum DATA_TYPE { DT_NONE, DT_CHAR, DT_BYTE, DT_SHORT, DT_WORD, DT_INT, DT_DWORD, DT_STRING, DT_FLOAT, DT_DOUBLE };
std::vector<int> CTblEditorBase::memory_DataTypes;
int CTblEditorBase::memory_iRowCount;

bool CTblEditorBase::SaveFile(const CString& path, std::map<int, std::vector<std::string>> newData) {

    std::string ConvertedSavePath = std::string(path);
    std::vector<uint8_t> byteData;

    // 1. get saved iDataTypeCount (4 byte)
    TRACE("memory_iDataTypeCount = %d", CTblEditorBase::memory_iDataTypeCount);
    DWORD iDataTypeCount = CTblEditorBase::memory_iDataTypeCount;
    byteData.insert(byteData.end(), reinterpret_cast<uint8_t*>(&iDataTypeCount), reinterpret_cast<uint8_t*>(&iDataTypeCount) + sizeof(iDataTypeCount));

    // 2. get saved DataTypes

    byteData.insert(
        byteData.end(),
        reinterpret_cast<uint8_t*>(CTblEditorBase::memory_DataTypes.data()),
        reinterpret_cast<uint8_t*>(CTblEditorBase::memory_DataTypes.data()) + CTblEditorBase::memory_DataTypes.size() * sizeof(int)
    );

    
    // 3. get saved Row numbers

    // 3. iRC (row count)
    //int iRC = memory_iRowCount;
    int iRC = static_cast<int>(newData.size());
    byteData.insert(byteData.end(), reinterpret_cast<uint8_t*>(&iRC), reinterpret_cast<uint8_t*>(&iRC) + sizeof(iRC));


    //add data
    
    for (int i = 0; i < iRC; ++i)
    {
        const std::vector<std::string>& row = newData[i];

        for (int j = 0; j < memory_iDataTypeCount; ++j)
        {
            DATA_TYPE dataType = static_cast<DATA_TYPE>(CTblEditorBase::memory_DataTypes[j]);
            const std::string& value = row[j];

            switch (dataType)
            {
            case DT_CHAR:
            {
                char val = static_cast<char>(std::stoi(value));
                byteData.insert(byteData.end(), reinterpret_cast<uint8_t*>(&val), reinterpret_cast<uint8_t*>(&val) + sizeof(char));
                break;
            }
            case DT_BYTE:
            {
                uint8_t val = static_cast<uint8_t>(std::stoi(value));
                byteData.insert(byteData.end(), reinterpret_cast<uint8_t*>(&val), reinterpret_cast<uint8_t*>(&val) + sizeof(uint8_t));
                break;
            }
            case DT_SHORT:
            {
                short val = static_cast<short>(std::stoi(value));
                byteData.insert(byteData.end(), reinterpret_cast<uint8_t*>(&val), reinterpret_cast<uint8_t*>(&val) + sizeof(short));
                break;
            }
            case DT_WORD:
            {
                uint16_t val = static_cast<uint16_t>(std::stoi(value));
                byteData.insert(byteData.end(), reinterpret_cast<uint8_t*>(&val), reinterpret_cast<uint8_t*>(&val) + sizeof(uint16_t));
                break;
            }
            case DT_INT:
            {
                int val = std::stoi(value);
                byteData.insert(byteData.end(), reinterpret_cast<uint8_t*>(&val), reinterpret_cast<uint8_t*>(&val) + sizeof(int));
                break;
            }
            case DT_DWORD:
            {
                uint32_t val = static_cast<uint32_t>(std::stoul(value));
                byteData.insert(byteData.end(), reinterpret_cast<uint8_t*>(&val), reinterpret_cast<uint8_t*>(&val) + sizeof(uint32_t));
                break;
            }
            case DT_STRING:
            {
                int len = static_cast<int>(value.length());
                byteData.insert(byteData.end(), reinterpret_cast<uint8_t*>(&len), reinterpret_cast<uint8_t*>(&len) + sizeof(int));
                byteData.insert(byteData.end(), value.begin(), value.end());
                break;
            }
            case DT_FLOAT:
            {
                float val = std::stof(value);
                byteData.insert(byteData.end(), reinterpret_cast<uint8_t*>(&val), reinterpret_cast<uint8_t*>(&val) + sizeof(float));
                break;
            }
            case DT_DOUBLE:
            {
                double val = std::stod(value);
                byteData.insert(byteData.end(), reinterpret_cast<uint8_t*>(&val), reinterpret_cast<uint8_t*>(&val) + sizeof(double));
                break;
            }
            case DT_NONE:
            default:
                // Veri yok, hiçbir şey yazma
                break;
            }
        }
    }


    if (ConvertedSavePath.empty())
        return false;

    // Create or open the file for writing
    HANDLE hFile = ::CreateFile(ConvertedSavePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (INVALID_HANDLE_VALUE == hFile)
    {
        return false;
    }

    // Encryption key as defined earlier
    uint16_t key_r = 0x0816;
    uint16_t key_c1 = 0x6081;
    uint16_t key_c2 = 0x1608;

    DWORD dwRWC = 0;

    // Encrypt the data before writing it to the file
    size_t dataSize = byteData.size();
    uint8_t* encryptedData = new uint8_t[dataSize];

    for (size_t i = 0; i < dataSize; i++)
    {

        uint8_t cipher;
        cipher = (byteData[i] ^ (key_r >> 8));
        key_r = (cipher + key_r) * key_c1 + key_c2;
        encryptedData[i] = cipher;
        /*
        uint8_t byData = (pDataToWrite[i] ^ (key_r >> 8));
        key_r = (pDataToWrite[i] + key_r) * key_c1 + key_c2;
        encryptedData[i] = byData;
        */
    }
    TRACE("İlk şifreli byte: %02X\n", encryptedData[0]);

    TRACE("Kaydedilecek veri boyutu: %zu\n", dataSize);

    // Write encrypted data to the file
    BOOL bResult = ::WriteFile(hFile, encryptedData, (DWORD)dataSize, &dwRWC, NULL);


    TRACE("Gerçek yazılan byte sayısı: %lu\n", dwRWC);
    // Clean up
    delete[] encryptedData;
    CloseHandle(hFile);

    return (bResult && dwRWC == dataSize);

}

bool CTblEditorBase::LoadFile(const CString& path) {

	//convert CString to std::string

	std::string ConvertedPath = std::string(path);

	//check if path is empty or not 

	if (ConvertedPath.empty())
	{
		TRACE("Path is empty !");
		return false;
	}

	HANDLE hFile = ::CreateFile(ConvertedPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{

		TRACE("\nerror:INVALID HANDLE VALUE\n");

		return false;
	}

	//set temporary path
	std::string TemporaryPath = ConvertedPath + ".tmp";
	TRACE("\nTemporary Path:'%s'\n", TemporaryPath.c_str());

	DWORD dwSizeHigh = 0;
	DWORD dwSizeLow = ::GetFileSize(hFile, &dwSizeHigh);
	if (dwSizeLow <= 0)
	{
		TRACE("\nerror:dwSizeLow\n");

		CloseHandle(hFile);
		::remove(TemporaryPath.c_str());
		return false;
	}

	// define pDatas with respect to dwSizeLow ( max size of document in bytes )
	uint8_t* pDatas = new uint8_t[dwSizeLow];
	DWORD dwRWC = 0;
	::ReadFile(hFile, pDatas, dwSizeLow, &dwRWC, NULL);
	CloseHandle(hFile); // close original file

	// same key with the one used in table creator 
	uint16_t key_r = 0x0816;
	uint16_t key_c1 = 0x6081;
	uint16_t key_c2 = 0x1608;

	// decrypt
	for (uint32_t i = 0; i < dwSizeLow; i++)
	{
		uint8_t byData = (pDatas[i] ^ (key_r >> 8));
		key_r = (pDatas[i] + key_r) * key_c1 + key_c2;
		pDatas[i] = byData;
	}

	// Open temporary file szFNTmp in writing mode.
	hFile = ::CreateFile(TemporaryPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	::WriteFile(hFile, pDatas, dwSizeLow, &dwRWC, NULL); // write encrypted data into temporary file
	CloseHandle(hFile); // 임시 파일 닫기
	delete[] pDatas; pDatas = NULL;

	// open temporary file in reading mode
	hFile = ::CreateFile(TemporaryPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//try loading file after cryption
    std::map<int, std::vector<std::string>> Data;

	SetRowData(hFile);

	CloseHandle(hFile);

	// delete temporary file
	::remove(TemporaryPath.c_str());

	return true;
}

void CTblEditorBase::SetRowData(HANDLE hFile) {
	
	// Reading how the structure of the data (column) is organized.
	DWORD dwNum;
	int i, j, iDataTypeCount = 0;

	// Read first 4 bytes = number of columns in the table (datatype count)
	if (!ReadFile(hFile, &iDataTypeCount, sizeof(int), &dwNum, NULL) || dwNum != sizeof(int))
	{
		TRACE("Failed to read datatype count.\n");
		//return false;
	}

	TRACE("Columns: %d\n", iDataTypeCount);

	if (iDataTypeCount <= 0)
	{
		TRACE("Invalid column count.\n");
		//return false;
	}

    //save this for later use
    CTblEditorBase::memory_iDataTypeCount = iDataTypeCount;

	// Read the data types, starting at byte 4 and reading (4 * iDataTypeCount) bytes
	std::vector<int> dataTypes(iDataTypeCount);

	if (!ReadFile(hFile, dataTypes.data(), iDataTypeCount * sizeof(int), &dwNum, NULL) || dwNum != iDataTypeCount * sizeof(int))
	{
		TRACE("Failed to read data types.\n");
		//return false;
	}

	// Optional: Print data types for debug
	for (int i = 0; i < iDataTypeCount; ++i)
	{
		TRACE("DataType[%d] = %d\n", i, dataTypes[i]);
	}

    CTblEditorBase::memory_DataTypes = dataTypes;

	// Now read the row count (4 bytes after the dataTypes array)
	int iRowCount = 0;
	if (!ReadFile(hFile, &iRowCount, sizeof(int), &dwNum, NULL) || dwNum != sizeof(int))
	{
		TRACE("Failed to read row count.\n");
		//return false;
	}

	TRACE("Row Count: %d\n", iRowCount);
    CTblEditorBase::memory_iRowCount = iRowCount;

    // Continue from where the rowCount has been read
// dataTypes ve rowCount başarıyla okundu, şimdi satır verisini çözebiliriz.

    // Satırları saklayacak bir vektör tanımlıyoruz.
    std::map<int, std::vector<std::string>> rowData;

    for (int i = 0; i < iRowCount; ++i)
    {
        std::vector<std::string> row;

        // Her bir satır verisi için veri okuma
        for (int j = 0; j < iDataTypeCount; ++j)
        {
            DATA_TYPE dataType = static_cast<DATA_TYPE>(dataTypes[j]); // Veri tipi enum

            switch (dataType)
            {
            case DT_NONE:
                // No data
                TRACE("Row %d, Column %d: DT_NONE (no data)\n", i, j);
                break;

            case DT_CHAR:
            {
                char charValue;
                if (!ReadFile(hFile, &charValue, sizeof(char), &dwNum, NULL) || dwNum != sizeof(char))
                {
                    TRACE("Failed to read DT_CHAR data at row %d, column %d\n", i, j);
                    //return false;
                }

                row.push_back(std::to_string(charValue));
                TRACE("Row %d, Column %d: DT_CHAR = %c\n", i, j, charValue);
                break;
            }

            case DT_BYTE:
            {
                uint8_t byteValue;
                if (!ReadFile(hFile, &byteValue, sizeof(uint8_t), &dwNum, NULL) || dwNum != sizeof(uint8_t))
                {
                    TRACE("Failed to read DT_BYTE data at row %d, column %d\n", i, j);
                    //return false;
                }

                row.push_back(std::to_string(byteValue));
                TRACE("Row %d, Column %d: DT_BYTE = %u\n", i, j, byteValue);
                break;
            }

            case DT_SHORT:
            {
                short shortValue;
                if (!ReadFile(hFile, &shortValue, sizeof(short), &dwNum, NULL) || dwNum != sizeof(short))
                {
                    TRACE("Failed to read DT_SHORT data at row %d, column %d\n", i, j);
                    //return false;
                }

                row.push_back(std::to_string(shortValue));

                TRACE("Row %d, Column %d: DT_SHORT = %d\n", i, j, shortValue);
                break;
            }

            case DT_WORD:
            {
                uint16_t wordValue;
                if (!ReadFile(hFile, &wordValue, sizeof(uint16_t), &dwNum, NULL) || dwNum != sizeof(uint16_t))
                {
                    TRACE("Failed to read DT_WORD data at row %d, column %d\n", i, j);
                    //return false;
                }

                row.push_back(std::to_string(wordValue));

                TRACE("Row %d, Column %d: DT_WORD = %u\n", i, j, wordValue);
                break;
            }

            case DT_INT:
            {
                int intValue;
                if (!ReadFile(hFile, &intValue, sizeof(int), &dwNum, NULL) || dwNum != sizeof(int))
                {
                    TRACE("Failed to read DT_INT data at row %d, column %d\n", i, j);
                    //return false;
                }

                row.push_back(std::to_string(intValue));
                TRACE("Row %d, Column %d: DT_INT = %d\n", i, j, intValue);
                break;
            }

            case DT_DWORD:
            {
                uint32_t dwordValue;
                if (!ReadFile(hFile, &dwordValue, sizeof(uint32_t), &dwNum, NULL) || dwNum != sizeof(uint32_t))
                {
                    TRACE("Failed to read DT_DWORD data at row %d, column %d\n", i, j);
                    //return false;
                }

                row.push_back(std::to_string(dwordValue));

                TRACE("Row %d, Column %d: DT_DWORD = %u\n", i, j, dwordValue);
                break;
            }

            case DT_STRING:
            {
                // Okunacak string'in uzunluğunu oku
                int iStrLen = 0;
                if (!ReadFile(hFile, &iStrLen, sizeof(iStrLen), &dwNum, NULL) || dwNum != sizeof(iStrLen))
                {
                    TRACE("Failed to read string length at row %d, column %d\n", i, j);
                    //return false;
                }

                TRACE("Row %d, Column %d: String length = %d\n", i, j, iStrLen);

                // Eğer string uzunluğu sıfırdan büyükse veriyi oku

                if (iStrLen > 0)
                {
                    std::string szString;
                    szString.assign(iStrLen, ' '); // Belirtilen uzunluk kadar boşlukla başlat

                    // Veriyi oku ve string'e yerleştir
                    if (!ReadFile(hFile, &(szString[0]), iStrLen, &dwNum, NULL) || dwNum != iStrLen)
                    {
                        TRACE("Failed to read string data at row %d, column %d\n", i, j);
                        //return false;
                    }

                    // String'i başarıyla okuduk
                    row.push_back(szString);
                    TRACE("Row %d, Column %d: DT_STRING = %s\n", i, j, szString.c_str());
                }
                else
                {
                    // Eğer string uzunluğu sıfırsa boş string
                    TRACE("Row %d, Column %d: DT_STRING = (empty string)\n", i, j);
                }

                break;
            }

            case DT_FLOAT:
            {
                float floatValue;
                if (!ReadFile(hFile, &floatValue, sizeof(float), &dwNum, NULL) || dwNum != sizeof(float))
                {
                    TRACE("Failed to read DT_FLOAT data at row %d, column %d\n", i, j);
                    //return false;
                }


                row.push_back(std::to_string(floatValue));
                TRACE("Row %d, Column %d: DT_FLOAT = %f\n", i, j, floatValue);
                break;
            }

            case DT_DOUBLE:
            {
                double doubleValue;
                if (!ReadFile(hFile, &doubleValue, sizeof(double), &dwNum, NULL) || dwNum != sizeof(double))
                {
                    TRACE("Failed to read DT_DOUBLE data at row %d, column %d\n", i, j);
                    //return false;
                }

                row.push_back(std::to_string(doubleValue));

                TRACE("Row %d, Column %d: DT_DOUBLE = %f\n", i, j, doubleValue);
                break;
            }

            default:

                row.push_back("N/A"); // Tanımsız veri tipi için
                TRACE("Unknown data type at row %d, column %d\n", i, j);
                break;
            }

            rowData[i] = row;

        }
    }

	CTblEditorBase::Data = rowData;
}