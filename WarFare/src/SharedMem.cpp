// SharedMem.cpp: implementation of the CSharedMemQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SharedMem.h"
#include <process.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// DEFINE Shared Memory Queue Return VALUE

#define SIZE_RECEIVER	1024
#define SIZE_TRANSFER	1024

#define SMQ_KNIGHTSEND	"Ksend_Nrecv"
#define SMQ_KNIGHTRECV	"Krecv_Nsend"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSharedMemQueue::CSharedMemQueue()
{
	m_hrMMFile = NULL;
	m_hsMMFile = NULL;
	m_lpMMFile = NULL;
}

CSharedMemQueue::~CSharedMemQueue()
{
	fConnected = FALSE ;
	DestroySMQ();
}

DWORD WINAPI SmqWatchProc(LPVOID lpData)
{
	CSharedMemQueue* npSmq = (CSharedMemQueue*)lpData;
	while(npSmq->fConnected) {
		if ((npSmq->Rxd->Head) != (npSmq->Rxd->Tail)) {
			npSmq->SetReadData();
		}
	}
	return TRUE;
}

void CSharedMemQueue::DestroySMQ()
{
	if (m_lpMMFile) {
		fConnected = FALSE ;
		Txd->Connect = FALSE;
		if( m_lpMMFile )	UnmapViewOfFile(m_lpMMFile);
		if( m_hrMMFile )	CloseHandle(m_hrMMFile);
		if( m_hsMMFile )	CloseHandle(m_hsMMFile);
	}
}

void CSharedMemQueue::SetReadData()
{
	WORD		size;
	char	*InData = Rxd->Data + Rxd->Tail;
	if (*InData!=0x7f) {
		(Rxd->Tail) ++;
		TRACE("Start가 없음");
//		AfxMessageBox("Start가 없음", NULL, MB_OK);
		return;
	}
	memcpy((char *) &size, InData+1, 2);
	memcpy(abIn, InData+3, size);
	if (*(InData+size+3)!=0x7e)	
		TRACE("Stop가 없음");
//		AfxMessageBox("Stop가 없음",NULL, MB_OK);
	Rxd->Tail += size+4;
	SendMessage(m_hwnd,WM_RECEIVEDATA,size,0);
}

void CSharedMemQueue::GetReadData(LPSTR data, int nLength)
{
	memcpy(data,abIn,nLength);
}

BOOL CSharedMemQueue::IsPartner()
{
	return Rxd->Connect;
}

BOOL CSharedMemQueue::PutData(char *pBuf, WORD size)
{
	if (!Rxd->Connect) return FALSE;
	char	*OutData = Txd->Data + Txd->Head;
	*OutData = 0x7f;
	memcpy(OutData+1, (char *)&size, 2);
	memcpy(OutData+3, pBuf, size);
	*(OutData+size+3) = 0x7e;

	Txd->Head += size+4;
	return	TRUE;
}

//메세지를 전달할 hwnd설정
void CSharedMemQueue::SetHwnd(HWND hwnd)
{
	m_hwnd=hwnd;
}

BOOL CSharedMemQueue::CreateSmq(BOOL Server)
{
	DWORD dwfullsize = SIZE_RECEIVER+SIZE_TRANSFER+sizeof(_SMQ_HEADER)+100;
	if (Server) {
		m_hrMMFile = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, dwfullsize, SMQ_KNIGHTRECV);
		m_hsMMFile = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, dwfullsize, SMQ_KNIGHTSEND);
	}
	else {
		m_hrMMFile = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, dwfullsize, SMQ_KNIGHTSEND);
		m_hsMMFile = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, dwfullsize, SMQ_KNIGHTRECV);
	}
    if(!(m_lpMMFile = (char *)MapViewOfFile(m_hrMMFile, FILE_MAP_WRITE,0,0,0))) return FALSE;

	Rxd = (_SMQ_HEADER *)m_lpMMFile;
  	Rxd->Data  = m_lpMMFile+sizeof(_SMQ_HEADER);

    if(!(m_lpMMFile = (char *)MapViewOfFile(m_hsMMFile, FILE_MAP_WRITE,0,0,0))) return FALSE;

	Txd = (_SMQ_HEADER *)m_lpMMFile;
  	Txd->Data  = m_lpMMFile+sizeof(_SMQ_HEADER);
	Txd->Head = Txd->Tail = Rxd->Head = Rxd->Tail =0;

	//	m_hReadQueueThread = AfxBeginThread((AFX_THREADPROC)SmqWatchProc,(LPVOID)this);
	DWORD id;
	m_hReadQueueThread = ::CreateThread( NULL, 0, SmqWatchProc, (LPVOID)this, 0, &id);
	Txd->Connect = TRUE;
	fConnected = TRUE;
	return TRUE;
}
