// APISocket.cpp: implementation of the CAPISocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APISocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//
WSAData		CAPISocket::s_WSData;
int			CAPISocket::s_nInstanceCount = 0;


#ifdef _CRYPTION
BOOL		DataPack::s_bCryptionFlag = FALSE;			//0 : 비암호화 , 1 : 암호화
//_int64		DataPack::s_PublicKey;
//_int64		DataPack::s_PrivateKey = 0x1234567890123456;
CJvCryption	DataPack::s_JvCrypt;
WORD		DataPack::s_wSendVal = 0;
WORD		DataPack::s_wRcvVal = 0;
#endif




//#define STX 1007
//#define ETX 1005
const WORD PACKET_HEADER = 0XAA55;
const WORD PACKET_TAIL = 0X55AA;

#ifdef _N3GAME
#include "LogWriter.h"
#endif

#ifdef _DEBUG
#include "PacketDef.h"
#endif










#ifdef _CRYPTION
// bSend가 TRUE이면 Encrypt, FALSE이면 Decrypt해준다.
DataPack::DataPack(int size, BYTE *pData, BOOL bSend)
{
	static BYTE pTIBuf[RECEIVE_BUF_SIZE];
	static BYTE pTBuf[RECEIVE_BUF_SIZE];
	__ASSERT(size, "size is 0");
	if (TRUE == s_bCryptionFlag)
	{
		if (bSend)
		{	// 서버로 보낼것 (일반 -> 암호화된것)
			// inmate - cryption
			if(TRUE == s_bCryptionFlag)
			{
				int clyp_size = size + (sizeof(WORD)+1+1);

				++s_wSendVal;

				pTIBuf[0] = 0xfc; // 암호가 정확한지
				memcpy( pTIBuf+1, &s_wSendVal, sizeof(WORD) );
				pTIBuf[3] = 0x00;
				memcpy( pTIBuf+4, pData, size );
				s_JvCrypt.JvEncryptionFast( clyp_size, pTIBuf, pTBuf );

				BYTE* pTmp = pTIBuf;
				
				m_Size = clyp_size;
				m_pData = new BYTE[m_Size+1];
				CopyMemory(m_pData, pTBuf, m_Size);
			}
			else
			{
				m_Size = size;
				m_pData = new BYTE[m_Size+1];
				CopyMemory(m_pData, pData, m_Size);
			}
		}
		else
		{	// 서버로부터 받은 데이타(암호화된것 -> 일반)
			s_JvCrypt.JvDecryptionFast( size, pData, pTBuf );
			if(pTBuf[0] != 0xfc) // 압축 푼 데이터 오류 일경우
			{
				m_Size = 0;
				m_pData = NULL;
				__ASSERT(0, "Crypt Error");
			}
			else
			{
				m_Size = size-4;
				m_pData = new BYTE[m_Size+1];
				CopyMemory(m_pData, &(pTBuf[4]), m_Size);
				m_pData[m_Size] = '\0';
			}
		}
	}
	else
	{	// 암호화가 아니다.
		m_Size = size;
		m_pData = new BYTE[size+1];
		CopyMemory(m_pData, pData, size);
	}
}
#endif






CAPISocket::CAPISocket()
{
	m_hSocket = INVALID_SOCKET;
	m_hWndTarget = NULL;
	m_szIP = "";
	m_dwPort = 0;

	if (s_nInstanceCount == 0)
	{
		WSAStartup(0x0101, &s_WSData);
	}
	s_nInstanceCount++;

	m_iSendByteCount = 0;
	m_bConnected = FALSE;
	m_bEnableSend = TRUE; // 보내기 가능..?
}

CAPISocket::~CAPISocket()
{
	Release();

	s_nInstanceCount--;
	if (s_nInstanceCount==0)
	{
		WSACleanup();
	}
}

void CAPISocket::Release()
{
	this->Disconnect();

	DataPack* pBuf;
	while ( m_qRecvPkt.size() > 0 )
	{
		pBuf = m_qRecvPkt.front();
		delete pBuf;
		m_qRecvPkt.pop();
	}

	m_iSendByteCount = 0;

	// 통계를 써준다..
#ifdef _DEBUG
/*	DWORD dwRWC = 0;
	char szFN1[256] = "", szFN2[256] = "";
	SYSTEMTIME ST;
	::GetLocalTime(&ST);
	sprintf(szFN1, "Socket_Statistics_Send_%d_%d_%d_%d.txt", ST.wMonth, ST.wDay, ST.wHour, ST.wMinute);
	sprintf(szFN2, "Socket_Statistics_Recv_%d_%d_%d_%d.txt", ST.wMonth, ST.wDay, ST.wHour, ST.wMinute);
	HANDLE hFile1 = ::CreateFile(szFN1, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hFile2 = ::CreateFile(szFN2, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	
	char szBuff[64] = "";
	char szCmd[32] = "";

	strcpy(szBuff, "Packet\t양\t횟수\r\n");
	WriteFile(hFile1, szBuff, lstrlen(szBuff), &dwRWC, NULL);
	WriteFile(hFile2, szBuff, lstrlen(szBuff), &dwRWC, NULL);

	for(int i = 0; i < 255; i++)
	{
		if(i == N3_NPC_MOVE) lstrcpy(szCmd, "NPC Move");
		else if(i == N3_ATTACK) lstrcpy(szCmd, "Attack");
		else if(i == N3_MOVE) lstrcpy(szCmd, "User Move");
		else if(i == N3_MAGIC) lstrcpy(szCmd, "Magic");
		else if(i == N3_CHAT) lstrcpy(szCmd, "Chatting");
		else 
		{
			sprintf(szCmd, "ETC : %d", i);
		}

		if(m_Statistics_Send_Sum[i].iSize > 0 || m_Statistics_Send_Sum[i].dwTime > 0)
		{
			sprintf(szBuff, "%s\t%d\t%d\t\r\n", szCmd, m_Statistics_Send_Sum[i].iSize, m_Statistics_Send_Sum[i].dwTime);
			WriteFile(hFile1, szBuff, lstrlen(szBuff), &dwRWC, NULL);
		}

		if(m_Statistics_Recv_Sum[i].iSize > 0 || m_Statistics_Recv_Sum[i].dwTime > 0)
		{
			sprintf(szBuff, "%s\t%d\t%d\t\r\n", szCmd, m_Statistics_Recv_Sum[i].iSize, m_Statistics_Recv_Sum[i].dwTime);
			WriteFile(hFile2, szBuff, lstrlen(szBuff), &dwRWC, NULL);
		}
	}
*/
	
	for(int i = 0; i < 255; i++)
	{
		memset(m_Statistics_Send_Sum, 0, sizeof(m_Statistics_Send_Sum));
		memset(m_Statistics_Recv_Sum, 0, sizeof(m_Statistics_Recv_Sum));
	}

//	CloseHandle(hFile1);
//	CloseHandle(hFile2);
#endif

}

void CAPISocket::Disconnect()
{
	if (m_hSocket != INVALID_SOCKET)
		closesocket(m_hSocket);
	
	m_hSocket = NULL;
	m_hWndTarget = NULL;
	m_szIP = "";
	m_dwPort = 0;

	m_bConnected = FALSE;
	m_bEnableSend = TRUE; // 보내기 가능..?

#ifdef _CRYPTION
	DataPack::InitCrypt(0); // 암호화 해제..
#endif // #ifdef _CRYPTION
}

int CAPISocket::Connect(HWND hWnd, const char* pszIP, DWORD dwPort)
{	
	if (!pszIP || !dwPort) return -1;

	//
	if (m_hSocket != INVALID_SOCKET)
	{	
		this->Disconnect();
	}

	//
	int i=0;
	struct sockaddr_in far server;
	struct hostent far *hp;
  
	if( (pszIP[0] >= '0') && (pszIP[0] <= '9') ) 
	{
	   memset((char *) &server,0,sizeof(server));
	   server.sin_family      = AF_INET;
	   server.sin_addr.s_addr = inet_addr(pszIP);
	   server.sin_port        = htons((u_short)dwPort);
	}
	else
	{ 
		if ( (hp = (hostent far *)gethostbyname(pszIP)) == NULL)
		{
#ifdef _DEBUG
			char msg[256];
			sprintf(msg, "Error: Connecting to %s.",pszIP);
			MessageBox(hWnd, msg,"socket error", MB_OK | MB_ICONSTOP );
#endif
			return INVALID_SOCKET;
		}
		memset((char *) &server,0,sizeof(server));
		memcpy((char *) &server.sin_addr,hp->h_addr,hp->h_length);
		server.sin_family = hp->h_addrtype;
		server.sin_port = htons((u_short)dwPort);  
	}// else 

	// create socket 
	if( (m_hSocket = socket(AF_INET, SOCK_STREAM, 0)) < 1) 
	{
		int iErrCode = ::WSAGetLastError();
#ifdef _DEBUG
		char msg[256];
		sprintf(msg,"Error opening stream socket");
		MessageBox(hWnd, msg,"socket error", MB_OK | MB_ICONSTOP);
#endif
		return iErrCode;
	}

	// 소켓 옵션
	int iRecvBufferLen = RECEIVE_BUF_SIZE;
	int iErr = setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&iRecvBufferLen, 4);
  
	if (connect(m_hSocket, (struct sockaddr far *)&server, sizeof(server)) != 0) 
	{
		int iErrCode = ::WSAGetLastError();

		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;

#ifdef _DEBUG
//		char msg[256];
//		sprintf(msg,"Cannot connect to %s on Port %u : ErrorCode : %d", pszIP, dwPort, iErrCode);
//		MessageBox(hWnd, msg,"socket error", MB_OK | MB_ICONSTOP);
#endif
		return iErrCode;
	}

//	WSAAsyncSelect(m_hSocket, hWnd, WM_SOCKETMSG, FD_CONNECT | FD_ACCEPT | FD_READ | FD_CLOSE);
	WSAAsyncSelect(m_hSocket, hWnd, WM_SOCKETMSG, FD_CONNECT | FD_READ | FD_CLOSE);

	m_hWndTarget = hWnd;
	m_szIP = pszIP;
	m_dwPort = dwPort;
	m_bConnected = TRUE;
	
#ifdef _DEBUG
	for(i = 0; i < 255; i++)
	{
		memset(m_Statistics_Send_Sum, 0, sizeof(m_Statistics_Send_Sum));
		memset(m_Statistics_Recv_Sum, 0, sizeof(m_Statistics_Recv_Sum));
	}
#endif
	
	return 0;
}

int	CAPISocket::ReConnect()
{
	std::string szIP = m_szIP;
	return this->Connect(m_hWndTarget, szIP.c_str(), m_dwPort);
}

void CAPISocket::Receive()
{
	if (INVALID_SOCKET == m_hSocket || FALSE == m_bConnected)	return;

	DWORD	dwPktSize;
	DWORD	dwRead = 0;
	int		count = 0;

	ioctlsocket(m_hSocket, FIONREAD, &dwPktSize);
	while(dwRead < dwPktSize)
	{
		count = recv(m_hSocket, (char*)m_RecvBuf, RECEIVE_BUF_SIZE, 0);
		if (count == SOCKET_ERROR)
		{
			__ASSERT(0,"socket receive error!");
#ifdef _N3GAME
			int iErr = ::GetLastError();
			CLogWriter::Write("socket receive error! : %d", iErr);
			TRACE("socket receive error! : %d\n", iErr);
#endif
			break;
		}
		if (count)
		{
			dwRead += count;
			m_CB.PutData(m_RecvBuf, count);
		}
	}

	// packet analysis.
	while(ReceiveProcess());
}

BOOL CAPISocket::ReceiveProcess()
{
	int iCount = m_CB.GetValidCount();
	BOOL bFoundTail = FALSE;
	if (iCount >=7 )
	{
		BYTE *pData = new BYTE[iCount];
		m_CB.GetData(pData, iCount);
		int head_inc_size = 0;

		if ( PACKET_HEADER == ntohs(*((WORD*)pData)) )
		{
			short siCore = *((short*)(pData+2));
			if ( siCore <= iCount )
			{
				if ( PACKET_TAIL == ntohs(*((WORD*)(pData+iCount-2))) ) // 패킷 꼬리 부분 검사..
				{
					
#ifdef _CRYPTION
					DataPack *pDP = new DataPack(siCore, pData+4, FALSE);
#else
					DataPack *pDP = new DataPack(siCore, pData+4);
#endif

					m_qRecvPkt.push(pDP);
					m_CB.HeadIncrease(siCore + 6); // 환형 버퍼 인덱스 증가 시키기..
					bFoundTail = TRUE;
#ifdef _DEBUG
					BYTE byCmd = pData[4];
					m_Statistics_Recv_Sum[byCmd].dwTime++;
					m_Statistics_Recv_Sum[byCmd].iSize += siCore;
#endif
				}
			}
		}
		else
		{
			// 패킷이 깨졌다??
			__ASSERT(0, "broken packet header.. skip!");
			m_CB.HeadIncrease(iCount); // 환형 버퍼 인덱스 증가 시키기..
		}

		delete[] pData, pData = NULL;
	}

	return bFoundTail;
}

void CAPISocket::Send(BYTE* pData, int nSize)
{
	if(!m_bEnableSend) return; // 보내기 가능..?
	if (INVALID_SOCKET == m_hSocket || FALSE == m_bConnected)	return;


	
#ifdef _CRYPTION
	DataPack DP(nSize, pData, TRUE);	// 암호화(s_bCryptionFlag가 FALSE일때는 암호화하지 않음)
	nSize = DP.m_Size;
	pData = DP.m_pData;
#endif
	
	
	
	
	
	int nTotalSize = nSize+6;
	BYTE *pSendData = m_RecvBuf;
	*((WORD*)pSendData) = htons(PACKET_HEADER);	pSendData+=2;
	*((WORD*)pSendData) = nSize;				pSendData+=2;
	memcpy(pSendData, pData, nSize);			pSendData += nSize;
	*((WORD*)pSendData) = htons(PACKET_TAIL);	pSendData+=2;

	int nSent = 0;
	int count = 0;
	while(nSent < nTotalSize)
	{
		count = send(m_hSocket, (char*)m_RecvBuf, nTotalSize, 0);
		if (count == SOCKET_ERROR)
		{
			__ASSERT(0,"socket send error!");
#ifdef _N3GAME
			int iErr = ::GetLastError();
			CLogWriter::Write("socket send error! : %d", iErr);
			TRACE("socket send error! : %d\n", iErr);
			PostQuitMessage(-1);
#endif
			break;
		}
		if (count)
		{
			nSent += count;
		}
	}

#ifdef _DEBUG
	BYTE byCmd = pData[0]; // 통계 넣기..

//	__SocketStatisics SS;
//	SS.dwTime = GetTickCount();
//	SS.iSize = nSize;
//	m_Statistics_Send[byCmd].push_back(SS);

	m_Statistics_Send_Sum[byCmd].dwTime++;
	m_Statistics_Send_Sum[byCmd].iSize += nSize;
#endif

	m_iSendByteCount += nTotalSize;
}

void CAPISocket::Parse_GetString(const BYTE* buf, int &iOffset, std::string& szString, int len)
{
	if (len > 0)
	{
		szString.assign(len, ' ');
		memcpy(&(szString[0]), buf + iOffset, len);
		iOffset += len;
	}
	else if(len == 0) szString = "";
	else 
	{
		szString = "";
		CLogWriter::Write("CAPISocket::Parse_GetString -> Invalid string length");
	}
}
