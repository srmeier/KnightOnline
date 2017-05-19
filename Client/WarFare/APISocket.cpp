// APISocket.cpp: implementation of the CAPISocket class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "APISocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//
WSAData		CAPISocket::s_WSData;
int			CAPISocket::s_nInstanceCount = 0;


#ifdef _CRYPTION
BOOL		CAPISocket::s_bCryptionFlag = FALSE;			//0 : 비암호화 , 1 : 암호화
CJvCryption	CAPISocket::s_JvCrypt;
uint32_t	CAPISocket::s_wSendVal = 0;
uint32_t	CAPISocket::s_wRcvVal = 0;
#endif

const uint16_t PACKET_HEADER = 0XAA55;
const uint16_t PACKET_TAIL = 0X55AA;

#ifdef _N3GAME
#include "LogWriter.h"
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
		if(i == WIZ_NPC_MOVE) lstrcpy(szCmd, "NPC Move");
		else if(i == WIZ_ATTACK) lstrcpy(szCmd, "Attack");
		else if(i == WIZ_MOVE) lstrcpy(szCmd, "User Move");
		else if(i == WIZ_MAGIC_PROCESS) lstrcpy(szCmd, "Magic");
		else if(i == WIZ_CHAT) lstrcpy(szCmd, "Chatting");
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
	InitCrypt(0); // 암호화 해제..
#endif // #ifdef _CRYPTION
}

int CAPISocket::Connect(HWND hWnd, const char* pszIP, uint32_t dwPort)
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
	return this->Connect(m_hWndTarget, m_szIP.c_str(), m_dwPort);
}

void CAPISocket::Receive()
{
	if (INVALID_SOCKET == m_hSocket || FALSE == m_bConnected)	return;

	u_long	dwPktSize;
	u_long	dwRead = 0;
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
			//TRACE("socket receive error! : %d\n", iErr);
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
		uint8_t *pData = new uint8_t[iCount];
		m_CB.GetData(pData, iCount);
		int head_inc_size = 0;

		if ( PACKET_HEADER == ntohs(*((uint16_t*)pData)) )
		{
			int16_t siCore = *((int16_t*)(pData+2));
			if ( siCore <= iCount )
			{
				if ( PACKET_TAIL == ntohs(*((uint16_t*)(pData+iCount-2))) ) // 패킷 꼬리 부분 검사..
				{
					
					DataPack *pDP = new DataPack;
					if (TRUE == s_bCryptionFlag)
					{
						static uint8_t pTBuf[RECEIVE_BUF_SIZE];
						s_JvCrypt.JvDecryptionFast(siCore, pData + 4, pTBuf);

						uint16_t sig = *(uint16_t*)pTBuf;

						if (sig != 0x1EFC)
						{
							__ASSERT(0, "Crypt Error");
						}
						else
						{
							uint16_t sequence = *(uint16_t*)&pTBuf[2];
							uint8_t  empty = pTBuf[4];
							uint8_t* payload = &pTBuf[5];

							pDP->m_Size = siCore - 5;
							pDP->m_pData = new uint8_t[pDP->m_Size];
							memcpy(pDP->m_pData, payload, pDP->m_Size);
						}
					}
					else
					{
						pDP->m_Size = siCore;
						pDP->m_pData = new uint8_t[siCore];
						memcpy(pDP->m_pData, pData + 4, siCore);
					}

					m_qRecvPkt.push(pDP);
					m_CB.HeadIncrease(siCore + 6); // 환형 버퍼 인덱스 증가 시키기..
					bFoundTail = TRUE;
#ifdef _DEBUG
					uint8_t byCmd = pData[4];
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

void CAPISocket::Send(uint8_t* pData, int nSize)
{
	if(!m_bEnableSend) return; // 보내기 가능..?
	if (INVALID_SOCKET == m_hSocket || FALSE == m_bConnected)	return;

#ifdef _CRYPTION
	DataPack DP;

	if (s_bCryptionFlag)
	{
		static uint8_t pTBuf[RECEIVE_BUF_SIZE];

		++s_wSendVal;

		memcpy(pTBuf, &s_wSendVal, sizeof(uint32_t));
		memcpy((pTBuf + 4), pData, nSize);

		*((Uint32*)(pTBuf + (nSize + 4))) = crc32(pTBuf, (nSize + 4), -1);

		s_JvCrypt.JvEncryptionFast((nSize + 4 + 4), pTBuf, pTBuf);

		DP.m_Size = (nSize + 4 + 4);
		DP.m_pData = new uint8_t[DP.m_Size];
		memcpy(DP.m_pData, pTBuf, DP.m_Size);

		nSize = DP.m_Size;
		pData = DP.m_pData;
	}
#endif
	
	int nTotalSize = nSize+6;
	uint8_t *pSendData = m_RecvBuf;
	*((uint16_t*)pSendData) = htons(PACKET_HEADER);	pSendData+=2;
	*((uint16_t*)pSendData) = nSize;				pSendData+=2;
	memcpy(pSendData, pData, nSize);			pSendData += nSize;
	*((uint16_t*)pSendData) = htons(PACKET_TAIL);	pSendData+=2;

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
			//TRACE("socket send error! : %d\n", iErr);
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
	uint8_t byCmd = pData[0]; // 통계 넣기..

//	__SocketStatisics SS;
//	SS.dwTime = GetTickCount();
//	SS.iSize = nSize;
//	m_Statistics_Send[byCmd].push_back(SS);

	m_Statistics_Send_Sum[byCmd].dwTime++;
	m_Statistics_Send_Sum[byCmd].iSize += nSize;
#endif

	m_iSendByteCount += nTotalSize;
}

void CAPISocket::Parse_GetString(const uint8_t* buf, int &iOffset, std::string& szString, int len)
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
