#include "stdafx.h"
#include "Define.h"//#include "defineTest.h"

/*
void GetString(Byte* tBuf, Byte* sBuf, int len, int& index)
{
	memcpy(tBuf, sBuf+index, len);
	index += len;
}

Byte GetByte(Byte* sBuf, int& index)
{
	int t_index = index;
	index++;
	return (Byte)(*(sBuf+t_index));
}

int GetShort(Byte* sBuf, int& index)
{
	index += 2;
	return *(short*)(sBuf+index-2);
}

unsigned int GetDWORD(Byte* sBuf, int& index)
{
	index += 4;
	return *(unsigned int*)(sBuf+index-4);
}

float Getfloat(Byte* sBuf, int& index)
{
	index += 4;
	return *(float*)(sBuf+index-4);
}

void SetString(Byte* tBuf, char* sBuf, int len, int& index)
{
	memcpy(tBuf+index, sBuf, len);
	index += len;
}

void SetByte(Byte* tBuf, Byte sByte, int& index)
{
	*(tBuf+index) = (Byte)sByte;
	index++;
}

void SetShort(Byte* tBuf, int sShort, int& index)
{
	short temp = (short)sShort;

	memcpy( tBuf+index, &temp, 2);
	index += 2;
}

void SetDWORD(Byte* tBuf, unsigned int sDWORD, int& index)
{
	memcpy( tBuf+index, &sDWORD, 4);
	index += 4;
}

void Setfloat ( Byte* tBuf, float sFloat, int& index )
{
	memcpy( tBuf+index, &sFloat, 4);
	index += 4;
}

int ParseSpace( Byte* tBuf, Byte* sBuf)
{
	int i = 0, index = 0;
	bool flag = false;
	
	while(sBuf[index] == ' ' || sBuf[index] == '\t')index++;
	while(sBuf[index] !=' ' && sBuf[index] !='\t' && sBuf[index] !=(Byte) 0){
		tBuf[i++] = sBuf[index++];
		flag = true;
	}
	tBuf[i] = 0;

	while(sBuf[index] == ' ' || sBuf[index] == '\t')index++;
	if(!flag) return 0;	
	return index;
}

std::string GetProgPath()
{
	char Buf[256], Path[256];
	char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

	//::GetModuleFileName(AfxGetApp()->m_hInstance, Buf, 256);
	//_splitpath(Buf,drive,dir,fname,ext);
	//strcpy(Path, drive);
	//strcat(Path, dir);
	//std::string _Path = Path;

	std::string _Path = "";

	return _Path;
}

void LogFileWrite(std::string logstr )
{

	std::string ProgPath, LogFileName;
	FILE* file;
	int loglength;

	ProgPath = GetProgPath();
	loglength = logstr.length();//logstr.GetLength();

	//LogFileName.Format("%s\\DBWorker.log", ProgPath);
	
	//file.Open( LogFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite );

	//file.SeekToEnd();
	//file.Write(logstr, loglength);
	//file.Close();
}
*/
