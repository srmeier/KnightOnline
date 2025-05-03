#include "StdAfx.h"
#include "TblEditorBase.h"
#include <vector>
#include <map>

struct WriteBuffer : std::vector<uint8_t>
{
	template <typename T>
	void append(
		const T* value)
	{
		append(value, sizeof(T));
	}

	void append(
		const void* value,
		size_t length)
	{
		insert(
			end(),
			reinterpret_cast<const uint8_t*>(value),
			reinterpret_cast<const uint8_t*>(value) + length);
	}
};

CTblEditorBase::CTblEditorBase()
{
}

CTblEditorBase::~CTblEditorBase()
{
}

bool CTblEditorBase::SaveFile(
	const CString& path,
	const std::map<int, std::vector<CString>>& newData)
{
	WriteBuffer writeBuffer;

	// 1. iDataTypeCount (4 bytes)
	int32_t iDataTypeCount = static_cast<int32_t>(m_DataTypes.size());

	TRACE("iDataTypeCount = %d", iDataTypeCount);
	writeBuffer.append<int32_t>(&iDataTypeCount);

	// 2. stored datatypes (4 bytes each)
	for (int32_t dataType : m_DataTypes)
		writeBuffer.append<int32_t>(&dataType);

	// 3. row count
	int32_t iRowCount = static_cast<int32_t>(newData.size());
	writeBuffer.append<int32_t>(&iRowCount);

	// 4. row data
	for (const auto& rowItr : newData)
	{
		const auto& row = rowItr.second;

		for (int colNo = 0; colNo < iDataTypeCount; colNo++)
		{
			DATA_TYPE dataType = m_DataTypes[colNo];
			const CString& value = row[colNo];

			switch (dataType)
			{
				case DT_CHAR:
				{
					char val = static_cast<char>(_ttoi(value));
					writeBuffer.append<char>(&val);
					break;
				}

				case DT_BYTE:
				{
					uint8_t val = static_cast<uint8_t>(_ttoi(value));
					writeBuffer.append<uint8_t>(&val);
					break;
				}

				case DT_SHORT:
				{
					int16_t val = static_cast<int16_t>(_ttoi(value));
					writeBuffer.append<int16_t>(&val);
					break;
				}

				case DT_WORD:
				{
					uint16_t val = static_cast<uint16_t>(_ttoi(value));
					writeBuffer.append<uint16_t>(&val);
					break;
				}

				case DT_INT:
				{
					int32_t val = _ttoi(value);
					writeBuffer.append<int32_t>(&val);
					break;
				}

				case DT_DWORD:
				{
					uint32_t val = static_cast<uint32_t>(_tcstoul(value, nullptr, 0));
					writeBuffer.append<uint32_t>(&val);
					break;
				}

				case DT_STRING:
				{
					std::string valueA = CT2A(value, CP_ACP); // TODO: better localisation support

					int32_t len = static_cast<int32_t>(valueA.length());
					writeBuffer.append<int32_t>(&len);
					writeBuffer.append(valueA.c_str(), len);
					break;
				}

				case DT_FLOAT:
				{
					float val = _tcstof(value, nullptr);
					writeBuffer.append<float>(&val);
					break;
				}

				case DT_DOUBLE:
				{
					double val = _tcstod(value, nullptr);
					writeBuffer.append<double>(&val);
					break;
				}
			}
		}
	}

	if (path.IsEmpty())
		return false;

	// Create or open the file for writing
	HANDLE hFile = ::CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	// Encryption key as defined earlier
	uint16_t key_r = 0x0816;
	uint16_t key_c1 = 0x6081;
	uint16_t key_c2 = 0x1608;

	DWORD dwRWC = 0;

	// Encrypt the data before writing it to the file
	size_t dataSize = writeBuffer.size();
	uint8_t* encryptedData = new uint8_t[dataSize];

	for (size_t i = 0; i < dataSize; i++)
	{
		uint8_t cipher = (writeBuffer[i] ^ (key_r >> 8));
		key_r = (cipher + key_r) * key_c1 + key_c2;
		encryptedData[i] = cipher;
	}

	TRACE("First encrypted byte: %02X\n", encryptedData[0]);
	TRACE("Data size to be written: %zu\n", dataSize);

	// Write encrypted data to the file
	BOOL bResult = ::WriteFile(hFile, encryptedData, (DWORD) dataSize, &dwRWC, nullptr);


	TRACE("Actual number of bytes written: %u\n", dwRWC);

	// Clean up
	delete[] encryptedData;
	CloseHandle(hFile);

	return (bResult && dwRWC == dataSize);
}

bool CTblEditorBase::LoadFile(
	const CString& path)
{
	if (path.IsEmpty())
	{
		TRACE("Path is empty");
		return false;
	}

	HANDLE hFile = ::CreateFile(path, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE("Failed to open file: '%s'\n", path);
		return false;
	}

	CString TemporaryPath = path + ".tmp";
	TRACE("Temporary Path: '%s'\n", TemporaryPath);

	DWORD dwSizeLow = ::GetFileSize(hFile, nullptr);
	if (dwSizeLow == 0)
	{
		TRACE("Error: Filesize is empty\n");

		CloseHandle(hFile);
		::_tremove(TemporaryPath);
		return false;
	}

	// define pDatas with respect to dwSizeLow ( max size of document in bytes )
	uint8_t* pDatas = new uint8_t[dwSizeLow];
	DWORD dwRWC = 0;
	::ReadFile(hFile, pDatas, dwSizeLow, &dwRWC, nullptr);
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

	// Open temporary file for writing the decrypted data to.
	hFile = ::CreateFile(TemporaryPath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		delete[] pDatas;
		return false;
	}

	::WriteFile(hFile, pDatas, dwSizeLow, &dwRWC, nullptr); // write encrypted data into temporary file
	CloseHandle(hFile); // 임시 파일 닫기
	delete[] pDatas;
	pDatas = nullptr;

	// Open temporary file 
	// try loading file after cryption
	hFile = ::CreateFile(TemporaryPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	LoadRowData(hFile);
	CloseHandle(hFile);

	// delete temporary file
	::_tremove(TemporaryPath);

	return true;
}

void CTblEditorBase::LoadRowData(
	HANDLE hFile)
{
	// Reading how the structure of the data (column) is organized.
	DWORD dwNum;
	int iDataTypeCount = 0;

	// Read first 4 bytes = number of columns in the table (datatype count)
	if (!ReadFile(hFile, &iDataTypeCount, sizeof(int), &dwNum, nullptr)
		|| dwNum != sizeof(int))
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

	// Read the data types
	m_DataTypes.clear();
	m_DataTypes.resize(iDataTypeCount);

	for (int i = 0; i < iDataTypeCount; i++)
	{
		int iDataType = 0;
		if (!ReadFile(hFile, &iDataType, sizeof(int), &dwNum, nullptr)
			|| dwNum != sizeof(int))
		{
			TRACE("Failed to read data types.\n");
			//return false;
			continue;
		}

		m_DataTypes[i] = static_cast<DATA_TYPE>(iDataType);

		// Optional: Print data types for debug
		TRACE("DataType[%d] = %d\n", i, iDataType);
	}

	// Now read the row count (4 bytes after the dataTypes array)
	int iRowCount = 0;
	if (!ReadFile(hFile, &iRowCount, sizeof(int), &dwNum, nullptr)
		|| dwNum != sizeof(int))
	{
		TRACE("Failed to read row count.\n");
		//return false;
	}

	TRACE("Row count: %d\n", iRowCount);

	// Now that we've read the datatypes and the row count, we can read the row data.
	m_Rows.clear();

	CString szValue;
	for (int iRowNo = 0; iRowNo < iRowCount; iRowNo++)
	{
		std::vector<CString> row;
		row.reserve(iDataTypeCount);

		// Read each column's data for this row
		for (int iColNo = 0; iColNo < iDataTypeCount; iColNo++)
		{
			DATA_TYPE dataType = static_cast<DATA_TYPE>(m_DataTypes[iColNo]);

			szValue.Empty();
			switch (dataType)
			{
				case DT_NONE:
					// No data
					TRACE("Row %d, Column %d: DT_NONE (no data)\n", iRowNo, iColNo);
					break;

				case DT_CHAR:
				{
					char val = '\0';
					if (!ReadFile(hFile, &val, sizeof(char), &dwNum, nullptr)
						|| dwNum != sizeof(char))
					{
						TRACE("Failed to read DT_CHAR data at row %d, column %d\n", iRowNo, iColNo);
						//return false;
					}

					szValue.Format(_T("%c"), val);
					row.push_back(szValue);

					TRACE("Row %d, Column %d: DT_CHAR = %c\n", iRowNo, iColNo, val);
					break;
				}

				case DT_BYTE:
				{
					uint8_t val = 0;
					if (!ReadFile(hFile, &val, sizeof(uint8_t), &dwNum, nullptr)
						|| dwNum != sizeof(uint8_t))
					{
						TRACE("Failed to read DT_BYTE data at row %d, column %d\n", iRowNo, iColNo);
						//return false;
					}

					szValue.Format(_T("%u"), val);
					row.push_back(szValue);

					TRACE("Row %d, Column %d: DT_BYTE = %u\n", iRowNo, iColNo, val);
					break;
				}

				case DT_SHORT:
				{
					int16_t val = 0;
					if (!ReadFile(hFile, &val, sizeof(int16_t), &dwNum, nullptr)
						|| dwNum != sizeof(int16_t))
					{
						TRACE("Failed to read DT_SHORT data at row %d, column %d\n", iRowNo, iColNo);
						//return false;
					}

					szValue.Format(_T("%d"), val);
					row.push_back(szValue);

					TRACE("Row %d, Column %d: DT_SHORT = %d\n", iRowNo, iColNo, val);
					break;
				}

				case DT_WORD:
				{
					uint16_t val = 0;
					if (!ReadFile(hFile, &val, sizeof(uint16_t), &dwNum, nullptr)
						|| dwNum != sizeof(uint16_t))
					{
						TRACE("Failed to read DT_WORD data at row %d, column %d\n", iRowNo, iColNo);
						//return false;
					}

					szValue.Format(_T("%u"), val);
					row.push_back(szValue);

					TRACE("Row %d, Column %d: DT_WORD = %u\n", iRowNo, iColNo, val);
					break;
				}

				case DT_INT:
				{
					int32_t val = 0;
					if (!ReadFile(hFile, &val, sizeof(int32_t), &dwNum, nullptr)
						|| dwNum != sizeof(int32_t))
					{
						TRACE("Failed to read DT_INT data at row %d, column %d\n", iRowNo, iColNo);
						//return false;
					}

					szValue.Format(_T("%d"), val);
					row.push_back(szValue);

					TRACE("Row %d, Column %d: DT_INT = %d\n", iRowNo, iColNo, val);
					break;
				}

				case DT_DWORD:
				{
					uint32_t val = 0;
					if (!ReadFile(hFile, &val, sizeof(uint32_t), &dwNum, nullptr)
						|| dwNum != sizeof(uint32_t))
					{
						TRACE("Failed to read DT_DWORD data at row %d, column %d\n", iRowNo, iColNo);
						//return false;
					}

					szValue.Format(_T("%u"), val);
					row.push_back(szValue);

					TRACE("Row %d, Column %d: DT_DWORD = %u\n", iRowNo, iColNo, val);
					break;
				}

				case DT_STRING:
				{
					// Read 32-bit string length
					int32_t iStrLen = 0;
					if (!ReadFile(hFile, &iStrLen, sizeof(int32_t), &dwNum, nullptr)
						|| dwNum != sizeof(int32_t))
					{
						TRACE("Failed to read string length at row %d, column %d\n", iRowNo, iColNo);
						//return false;
					}

					TRACE("Row %d, Column %d: String length = %d\n", iRowNo, iColNo, iStrLen);

					// Invalid string length
					if (iStrLen < 0)
					{
						TRACE("Row %d, Column %d: DT_STRING = (invalid string)\n", iRowNo, iColNo);
						//return false;
					}
					else
					{
						// Empty string (but still valid)
						if (iStrLen == 0)
						{
							// Eğer string uzunluğu sıfırsa boş string
							TRACE("Row %d, Column %d: DT_STRING = (empty string)\n", iRowNo, iColNo);
						}
						else
						{
							CStringA szString;
							if (!ReadFile(hFile, szString.GetBuffer(iStrLen), iStrLen, &dwNum, nullptr)
								|| dwNum != iStrLen)
							{
								// szString.ReleaseBuffer();
								TRACE("Failed to read string data at row %d, column %d\n", iRowNo, iColNo);
								//return false;
							}

							szString.ReleaseBuffer();
							szValue = CA2T(szString, CP_ACP); // TODO: better localisation support

							TRACE("Row %d, Column %d: DT_STRING = %s\n", iRowNo, iColNo, szString);
						}

						row.push_back(szValue);
					}
					break;
				}

				case DT_FLOAT:
				{
					float val = 0.0f;
					if (!ReadFile(hFile, &val, sizeof(float), &dwNum, nullptr)
						|| dwNum != sizeof(float))
					{
						TRACE("Failed to read DT_FLOAT data at row %d, column %d\n", iRowNo, iColNo);
						//return false;
					}

					szValue.Format(_T("%f"), val);
					row.push_back(szValue);

					TRACE("Row %d, Column %d: DT_FLOAT = %f\n", iRowNo, iColNo, val);
					break;
				}

				case DT_DOUBLE:
				{
					double val = 0.0;
					if (!ReadFile(hFile, &val, sizeof(double), &dwNum, nullptr)
						|| dwNum != sizeof(double))
					{
						TRACE("Failed to read DT_DOUBLE data at row %d, column %d\n", iRowNo, iColNo);
						//return false;
					}

					szValue.Format(_T("%f"), val);
					row.push_back(szValue);

					TRACE("Row %d, Column %d: DT_DOUBLE = %f\n", iRowNo, iColNo, val);
					break;
				}

				default:
					row.push_back("N/A"); // Unknown data type
					TRACE("Unknown data type at row %d, column %d\n", iRowNo, iColNo);
					break;
			}
		}

		m_Rows.insert(std::make_pair(iRowNo, row));
	}
}
