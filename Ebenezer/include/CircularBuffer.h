// CircularBuffer.h: interface for the CCircularBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIRCULARBUFFER_H__F4D345A4_CE05_11D1_8BEE_0060979C5900__INCLUDED_)
#define AFX_CIRCULARBUFFER_H__F4D345A4_CE05_11D1_8BEE_0060979C5900__INCLUDED_

#include "Define.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CCircularBuffer  
{
public:
	CCircularBuffer(int size=4096);
	virtual ~CCircularBuffer();

	void	PutData(char *pData, int len);
	void	GetData(char *pData, int len);
	int		GetOutData(char *pData); //HeadPos, 변화
	void	PutData(char& data);
	char&	GetHeadData(){return m_pBuffer[m_iHeadPos];}
	//1 Byte Operation;
	//false : 모든데이터 다빠짐, TRUE: 정상적으로 진행중
	bool	HeadIncrease(int increasement=1);
	void	SetEmpty() {m_iHeadPos=0; m_iTailPos=0;}

	int&	GetBufferSize() {return m_iBufSize;}
	int&	GetHeadPos() {return m_iHeadPos;}
	int&	GetTailPos() {return m_iTailPos;}
	int		GetValidCount();
protected:
	//over flow 먼저 점검한 후 IndexOverFlow 점검
	bool	IsOverFlowCondition(int &len) {return (len >= m_iBufSize-GetValidCount()) ? true: false;}
	bool	IsIndexOverFlow(int &len) {return (len+m_iTailPos>=m_iBufSize) ? true:false;}
	void	BufferResize(); //overflow condition 일때 size를 현재의 두배로 늘림
protected:
	int		m_iBufSize;
	char	*m_pBuffer;

	int		m_iHeadPos;
	int		m_iTailPos;
};

inline int CCircularBuffer::GetValidCount()
{
	int count = m_iTailPos-m_iHeadPos;
	if (count<0) count = m_iBufSize+count;
	return count;
}

inline void CCircularBuffer::BufferResize()
{
	int prevBufSize = m_iBufSize;
	m_iBufSize <<= 1;
	char *pNewData = new char[m_iBufSize];
	memcpy(pNewData, m_pBuffer, prevBufSize);
	if (m_iTailPos<m_iHeadPos) 
	{
		memcpy(pNewData+prevBufSize, m_pBuffer, m_iTailPos);
		m_iTailPos += prevBufSize;
	}
	delete [] m_pBuffer;
	m_pBuffer = pNewData;
}

inline void CCircularBuffer::PutData(char &data)
{
	int len = 1;
	while (IsOverFlowCondition(len)) BufferResize();
	m_pBuffer[m_iTailPos++] = data;
	if (m_iTailPos==m_iBufSize) m_iTailPos = 0;
}

inline void CCircularBuffer::PutData(char *pData, int len)
{
	if (len<=0) {printf("CCircularBuffer::PutData len is <=0\n"); return;}
	while (IsOverFlowCondition(len)) BufferResize();
	if (IsIndexOverFlow(len))
	{
		int FirstCopyLen = m_iBufSize-m_iTailPos;
		int SecondCopyLen = len - FirstCopyLen;
		//ASSERT(FirstCopyLen);
		memcpy(m_pBuffer+m_iTailPos, pData, FirstCopyLen);
		if (SecondCopyLen)
		{
			memcpy(m_pBuffer, pData+FirstCopyLen, SecondCopyLen);
			m_iTailPos = SecondCopyLen;
		}
		else m_iTailPos = 0;
	}
	else
	{
		memcpy(m_pBuffer+m_iTailPos, pData, len);
		m_iTailPos += len;
	}
}

inline int CCircularBuffer::GetOutData(char *pData)
{
	int len = GetValidCount();
	int fc, sc;
	fc = m_iBufSize-m_iHeadPos;
	if (len>fc)
	{
		sc = len - fc;
		memcpy(pData, m_pBuffer+m_iHeadPos, fc);
		memcpy(pData+fc, m_pBuffer, sc);
		m_iHeadPos = sc;
		//ASSERT(m_iHeadPos==m_iTailPos);
	}
	else
	{
		memcpy(pData, m_pBuffer+m_iHeadPos, len);
		m_iHeadPos += len;
		if (m_iHeadPos==m_iBufSize) m_iHeadPos = 0;
	}
	return len;
}

inline void CCircularBuffer::GetData(char *pData, int len)
{
	//ASSERT(len>0&&len<=GetValidCount());
	if (len < m_iBufSize-m_iHeadPos)
		memcpy(pData, m_pBuffer+m_iHeadPos, len);
	else
	{
		int fc, sc;
		fc = m_iBufSize-m_iHeadPos;
		sc = len - fc;
		memcpy(pData, m_pBuffer+m_iHeadPos, fc);
		if (sc) memcpy(pData+fc, m_pBuffer, sc);
	}
}

inline bool CCircularBuffer::HeadIncrease(int increasement)
{
	//ASSERT(increasement<=GetValidCount());
	m_iHeadPos += increasement;
	m_iHeadPos %= m_iBufSize;
	return m_iHeadPos!=m_iTailPos;
}

#endif // !defined(AFX_CIRCULARBUFFER_H__F4D345A4_CE05_11D1_8BEE_0060979C5900__INCLUDED_)
