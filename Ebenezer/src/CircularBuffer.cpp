// CircularBuffer.cpp: implementation of the CCircularBuffer class.
//
//////////////////////////////////////////////////////////////////////

//#include "Ebenezer.h"
#include "stdafx.h"
#include "CircularBuffer.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCircularBuffer::CCircularBuffer(int size)
{
	//ASSERT(size>0);
	m_iBufSize = size;
	m_pBuffer = new char[m_iBufSize];

	m_iHeadPos = 0;
	m_iTailPos = 0;
}

CCircularBuffer::~CCircularBuffer()
{
	//ASSERT(m_pBuffer);
	delete [] m_pBuffer;
	m_pBuffer = NULL;
}
