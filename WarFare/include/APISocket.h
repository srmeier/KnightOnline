// APISocket.h: interface for the CAPISocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APISOCKET_H__31D58152_3B8D_4CBD_BEB9_6BE23C4F0FFB__INCLUDED_)
#define AFX_APISOCKET_H__31D58152_3B8D_4CBD_BEB9_6BE23C4F0FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <winsock.h>

#include <queue>
#include <string>

#define WM_SOCKETMSG	(WM_USER+1)
#define RECEIVE_BUF_SIZE	262144 // 최대 버퍼..

#define _CRYPTION		// 암호화 사용
#ifdef _CRYPTION
#include "JvCryption.h"
#endif

class BB_CircularBuffer  
{
public:
	BB_CircularBuffer(int size=RECEIVE_BUF_SIZE)
	{
		__ASSERT(size>0, "1");
		m_iBufSize = size;
		m_pBuffer = new BYTE[m_iBufSize];

		m_iHeadPos = 0;
		m_iTailPos = 0;
	}

	virtual ~BB_CircularBuffer()
	{
		__ASSERT(m_pBuffer,"1");
		if (m_pBuffer)
			delete[] m_pBuffer, m_pBuffer = NULL;
	}

	void	PutData(BYTE *pData, int len)
	{
		if (len<=0)
		{
	#ifdef _DEBUG
			OutputDebugString("BB_CircularBuffer::PutData len is <= 0\n");
	#endif
			return;
		}
		while (IsOverFlowCondition(len)) BufferResize();
		if (IsIndexOverFlow(len))
		{
			int FirstCopyLen = m_iBufSize-m_iTailPos;
			int SecondCopyLen = len - FirstCopyLen;
			__ASSERT(FirstCopyLen, "1");
			CopyMemory(m_pBuffer+m_iTailPos, pData, FirstCopyLen);
			if (SecondCopyLen)
			{
				CopyMemory(m_pBuffer, pData+FirstCopyLen, SecondCopyLen);
				m_iTailPos = SecondCopyLen;
			}
			else m_iTailPos = 0;
		}
		else
		{
			CopyMemory(m_pBuffer+m_iTailPos, pData, len);
			m_iTailPos += len;
		}
	}
	void	GetData(BYTE *pData, int len)
	{
		__ASSERT(len>0&&len<=GetValidCount(),"GetData error");
		if (len < m_iBufSize-m_iHeadPos)
			CopyMemory(pData, m_pBuffer+m_iHeadPos, len);
		else
		{
			int fc, sc;
			fc = m_iBufSize-m_iHeadPos;
			sc = len - fc;
			CopyMemory(pData, m_pBuffer+m_iHeadPos, fc);
			if (sc) CopyMemory(pData+fc, m_pBuffer, sc);
		}
	}
	int		GetOutData(BYTE *pData) //HeadPos, 변화
	{
		int len = GetValidCount();
		int fc, sc;
		fc = m_iBufSize-m_iHeadPos;
		if (len>fc)
		{
			sc = len - fc;
			CopyMemory(pData, m_pBuffer+m_iHeadPos, fc);
			CopyMemory(pData+fc, m_pBuffer, sc);
			m_iHeadPos = sc;
			__ASSERT(m_iHeadPos==m_iTailPos, "1");
		}
		else
		{
			CopyMemory(pData, m_pBuffer+m_iHeadPos, len);
			m_iHeadPos += len;
			if (m_iHeadPos==m_iBufSize) m_iHeadPos = 0;
		}
		return len;
	}
	void	PutData(BYTE& data)
	{
		int len = 1;
		while (IsOverFlowCondition(len)) BufferResize();
		m_pBuffer[m_iTailPos++] = data;
		if (m_iTailPos==m_iBufSize) m_iTailPos = 0;
	}
	BYTE&	GetHeadData(){return m_pBuffer[m_iHeadPos];}
	//1 Byte Operation;
	//false : 모든데이터 다빠짐, TRUE: 정상적으로 진행중
	BOOL	HeadIncrease(int increasement=1)
	{
		__ASSERT(increasement<=GetValidCount(),"1");
		m_iHeadPos += increasement;
		m_iHeadPos %= m_iBufSize;
		return m_iHeadPos!=m_iTailPos;
	}
	void	SetEmpty() {m_iHeadPos=0; m_iTailPos=0;}

	int&	GetBufferSize() {return m_iBufSize;}
	int&	GetHeadPos() {return m_iHeadPos;}
	int&	GetTailPos() {return m_iTailPos;}
	int		GetValidCount()
	{
		int count = m_iTailPos-m_iHeadPos;
		if (count<0) count = m_iBufSize+count;
		return count;
	}
protected:
	//over flow 먼저 점검한 후 IndexOverFlow 점검
	BOOL	IsOverFlowCondition(int &len) {return (len >= m_iBufSize-GetValidCount()) ? TRUE: FALSE;}
	BOOL	IsIndexOverFlow(int &len) {return (len+m_iTailPos>=m_iBufSize) ? TRUE:FALSE;}
	void	BufferResize() //overflow condition 일때 size를 현재의 두배로 늘림
	{
		int prevBufSize = m_iBufSize;
		m_iBufSize <<= 1;
		BYTE *pNewData = new BYTE[m_iBufSize];
		CopyMemory(pNewData, m_pBuffer, prevBufSize);
		if (m_iTailPos<m_iHeadPos) 
		{
			CopyMemory(pNewData+prevBufSize, m_pBuffer, m_iTailPos);
			m_iTailPos += prevBufSize;
		}
		delete [] m_pBuffer;
		m_pBuffer = pNewData;
	}
protected:
	int		m_iBufSize;
	BYTE	*m_pBuffer;

	int		m_iHeadPos;
	int		m_iTailPos;
};

class DataPack
{
public:
	int		m_Size;
	BYTE*	m_pData;



#ifdef _CRYPTION
protected:
	static BOOL			s_bCryptionFlag;			//0 : 비암호화 , 1 : 암호화
//	static _int64		s_PublicKey;
//	static _int64		s_PrivateKey;				// = 0x1234567890123456;
	static CJvCryption	s_JvCrypt;
	static WORD			s_wSendVal;
	static WORD			s_wRcvVal;
public:
	static void			InitCrypt(_int64 PublicKey)
	{
		s_JvCrypt.Init();
		s_JvCrypt.SetPublicKey(PublicKey);
		s_JvCrypt.SetPrivateKey(_int64(0x1234567890123456));
		s_wSendVal = 0;
		s_wRcvVal = 0;
		if (0 != PublicKey) s_bCryptionFlag = TRUE;
		else s_bCryptionFlag = FALSE;
	}
#endif



public:
	DataPack()
	{
		m_Size = 0;
		m_pData = NULL;
	}
	DataPack(int size, BYTE *pData)
	{
		__ASSERT(size, "size is 0");		
		m_Size = size;
		m_pData = new BYTE[size];
		CopyMemory(m_pData, pData, size);
	}
#ifdef _CRYPTION
	DataPack(int size, BYTE *pData, BOOL bSend);
#endif
	virtual ~DataPack(){delete[] m_pData;}
};

#ifdef _DEBUG
struct __SocketStatisics
{
	DWORD dwTime;
	int iSize;
};
#include <vector>
#include <string>
#endif

class CAPISocket  
{
protected:
	SOCKET		m_hSocket;
	
	HWND		m_hWndTarget;
	std::string m_szIP;
	DWORD		m_dwPort;

	BYTE	m_RecvBuf[RECEIVE_BUF_SIZE];
	BOOL	m_bConnected;

	BB_CircularBuffer	m_CB;

#ifdef _DEBUG
	__SocketStatisics m_Statistics_Send_Sum[255];
	__SocketStatisics m_Statistics_Recv_Sum[255];
//	std::vector<__SocketStatisics> m_Statistics_Send[255];
//	std::vector<__SocketStatisics> m_Statistics_Recv[255];
#endif


public:
	static int			s_nInstanceCount;
	static WSADATA		s_WSData;
	
	int					m_iSendByteCount;
	std::queue< DataPack* >	m_qRecvPkt;

	BOOL	m_bEnableSend; // 보내기 가능..?
public:
	int		Connect(HWND hWnd, const char* pszIP, DWORD port);
	void	Disconnect();
	BOOL	IsConnected() { return m_bConnected; }
	int		ReConnect();

	std::string GetCurrentIP() { return m_szIP; }
	DWORD		GetCurrentPort() { return m_dwPort; }

	void	Release();	
	void	Receive();
	BOOL	ReceiveProcess();
	void	Send(BYTE* pData, int nSize);


	//패킷 만들기 함수
	static	void	MP_AddByte(BYTE *dest, int& iOffset, BYTE byte) { CopyMemory(dest+iOffset, &byte, 1); iOffset ++; }
	static	void	MP_AddShort(BYTE *dest, int& iOffset, short value) { CopyMemory(dest+iOffset, &value, 2); iOffset += 2; }
	static	void	MP_AddWord(BYTE *dest, int& offset, WORD value) { CopyMemory(dest+offset, &value, 2); offset += 2; }
	static	void	MP_AddDword(BYTE *dest, int &iOffset, DWORD dword) { CopyMemory(dest+iOffset, &dword, 4); iOffset += 4; }
	static	void	MP_AddFloat(BYTE *dest, int &iOffset, float value) { CopyMemory(dest+iOffset, &value, 4);	iOffset += 4; }
	static	void	MP_AddString(BYTE *dest, int &iOffset, const std::string& szString)
	{
		if(!szString.empty())
		{
			CopyMemory(dest+iOffset, &(szString[0]), szString.size());
			iOffset += szString.size();
		}

	}

	//패킷 Parsing 함수
	static	BYTE&		Parse_GetByte(const BYTE* buf, int &iOffset) { iOffset ++; return *(BYTE*)(buf+iOffset-1); }
	static	short&		Parse_GetShort(const BYTE* buf, int& iOffset) { iOffset += 2; return *(short*)(buf+iOffset-2); }
	static  WORD&		Parse_GetWord(const BYTE* buf, int &iOffset) { iOffset += 2; return *(PWORD)(buf+iOffset-2); }
	static	DWORD&		Parse_GetDword(const BYTE* buf, int &iOffset) { iOffset += 4; return *(DWORD*)(buf+iOffset-4); }
	static	float&		Parse_GetFloat(const BYTE* buf, int& iOffset) { iOffset += 4; return *(float*)(buf+iOffset-4); }
	static	void		Parse_GetString(const BYTE* buf, int &iOffset, std::string& szString, int len);
	static	__int64&	Parse_GetInt64(const BYTE* buf, int &iOffset) { iOffset += 8; return *(__int64*)(buf+iOffset-8); }

	CAPISocket();
	virtual ~CAPISocket();
};

#endif // !defined(AFX_APISOCKET_H__31D58152_3B8D_4CBD_BEB9_6BE23C4F0FFB__INCLUDED_)
