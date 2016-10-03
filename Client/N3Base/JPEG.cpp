// Jpeg.cpp: implementation of the CJpeg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxbase.h"
#include "Jpeg.h"

#include <math.h>

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJpeg::CJpeg()
{	
	int i;
	m_pData = NULL;
	for(i=0; i<20; i++)
		TbH[i].Flag = FALSE;
}

CJpeg::~CJpeg()
{
	int i;

	if(m_pData != NULL)
		delete [] m_pData;	

	for(i=0; i<20; i++)
		if(TbH[i].Flag)
		{
			delete [] TbH[i].HUFFCODE;
			delete [] TbH[i].HUFFSIZE;
			delete [] TbH[i].HUFFVAL;
		}

}

void CJpeg::LoadJPG(LPCSTR FileName)
{
	// File Open & Load Entire Contents //
	HFILE hFile = _lopen(FileName, OF_READWRITE);
	int FileSize = GetFileSize((HANDLE)hFile, NULL);
	m_pBuf = new BYTE[FileSize];
	_lread(hFile, m_pBuf, FileSize);
	_lclose(hFile);
	m_Index = 0;
	int Count=0;

	FindSOI();	
	pByte = &m_pBuf[m_Index];
	while(TRUE)
	{
		FindETC(); // DRI(Define Restart Interval)
		FindDHT(); // Huffman Table Loading
		FindDQT(); // Quantization Table Loading
		FindSOF(); // Frame Header Loading
		FindSOS(); // Scan Header Loading & Decoding
		
		if( (*pByte == 0xff) && (*(pByte+1) == 0xd9) ) // 끝을 만났을 때
			break;
		Count++;
		if(Count > 50) // Loop가 끝날 가능성이 거의 없을 때
			break;		
	}

	delete [] m_pBuf;	
}

void CJpeg::FindSOI()
{	
	while(!((m_pBuf[m_Index] == 0xff)&&(m_pBuf[m_Index+1] == 0xd8)))
		m_Index++;
	m_Index = m_Index + 2;		
}


void CJpeg::FindDHT()
{
	if( (m_pBuf[m_Index] == 0xff) && (m_pBuf[m_Index+1] == 0xc4) )
	{
		WORD SegSize = m_pBuf[m_Index+2]*256 + m_pBuf[m_Index+3];
		//버퍼의 현재 위치를 포인터로 설정한다.
		BYTE *p = &m_pBuf[m_Index+4];		

		do
		{
			int i, j, k, LASTK;
			int Num = 0;
			BYTE BITS[17];		
			BYTE Th = *p; // Table Number
			memcpy(BITS, p, 17);
			p = p + 17;
			//17개의 값을 모두 더해서 Num에 저장
			for(i=1; i<17; i++)
				Num = Num + BITS[i];		

			TbH[Th].Flag = TRUE;
			
			TbH[Th].HUFFCODE = new WORD[Num+1];			
			TbH[Th].HUFFSIZE = new BYTE[Num+1];
			TbH[Th].HUFFVAL = new BYTE[Num+1];
			//Huffman Value 값을 Num크기만큼 p에서 읽는다.
			memcpy(TbH[Th].HUFFVAL, p, Num);			
			//p가 증가
			p = p + Num;

			
			// Generation of table of Huffman code sizes //
			i=1; j=1; k=0;
			while(i<=16)
			{
				while(j<=BITS[i])
				{
					TbH[Th].HUFFSIZE[k] = (BYTE)i;				
					k++;
					j++;
				}

				i++;
				j=1;
			}
			TbH[Th].HUFFSIZE[k] = 0;
			LASTK = k;	

			
			// Generation of table of Huffman codes
			k=0;
			WORD CODE = 0; 
			BYTE SI = TbH[Th].HUFFSIZE[0];		
			
			while(TRUE)
			{
				do
				{
					TbH[Th].HUFFCODE[k] = CODE;					
					CODE++;
					k++;
				}while(TbH[Th].HUFFSIZE[k] == SI);
				if(TbH[Th].HUFFSIZE[k] == 0)
					break;
				do
				{
					CODE = CODE << 1;
					SI++;
				}while(TbH[Th].HUFFSIZE[k] != SI);
			}
			


			// Decoder table generation //
			i=0; j=0;		
			while(TRUE)
			{
				do
				{
					i++;
					if(i>16)
						break;
					if(BITS[i] == 0)
						TbH[Th].MAXCODE[i] = -1;
				}while(BITS[i] == 0);
				if(i>16)
					break;
				TbH[Th].VALPTR[i] = j;
				TbH[Th].MINCODE[i] = TbH[Th].HUFFCODE[j];
				j = j + BITS[i] - 1;
				TbH[Th].MAXCODE[i] = TbH[Th].HUFFCODE[j];
				j++;
			}
			TbH[Th].Num = Num;		
		}while(*p != 0xff);

		m_Index = m_Index + SegSize + 2;
	}
}


void CJpeg::FindDQT()
{
	if( (m_pBuf[m_Index] == 0xff) && (m_pBuf[m_Index+1] == 0xdb) )
	{		
		WORD SegSize = m_pBuf[m_Index+2]*256 + m_pBuf[m_Index+3];
		
		BYTE *p = &m_pBuf[m_Index+4];

		do
		{
			BYTE Tq = *(p++) & 0x0f; // Table Number
			memcpy(TbQ[Tq].Q, &m_pBuf[m_Index+5], 64);
			p = p + 64;
		}while(*p != 0xff);		
		
		m_Index = m_Index + SegSize + 2;
	}
}

void CJpeg::FindSOF()
{
	if((m_pBuf[m_Index] == 0xff)&&(m_pBuf[m_Index + 1] == 0xc0))
	{
		int i;
		WORD SegSize = m_pBuf[m_Index + 2] * 256 + m_pBuf[m_Index + 3];
		FrameHeader.Y = m_pBuf[m_Index + 5] * 256 + m_pBuf[m_Index + 6];
		FrameHeader.X = m_pBuf[m_Index + 7] * 256 + m_pBuf[m_Index + 8];
		FrameHeader.Nf = m_pBuf[m_Index + 9];
		
		for(i=0; i<FrameHeader.Nf; i++)
		{
			FrameHeader.C[i] = m_pBuf[m_Index + 10 + 3*i];
			FrameHeader.H[i] = m_pBuf[m_Index + 11 + 3*i] >> 4;
			FrameHeader.V[i] = m_pBuf[m_Index + 11 + 3*i] & 0x0f;
			FrameHeader.Tq[i] = m_pBuf[m_Index + 12 + 3*i];
		}

		m_Index = m_Index + SegSize + 2;		
	}
}

void CJpeg::FindETC()
{
	if((m_pBuf[m_Index] == 0xff)&& (( (m_pBuf[m_Index + 1] & 0xf0) == 0xe0) || (m_pBuf[m_Index + 1] & 0xf0) == 0xf0))
	{
		WORD SegSize = m_pBuf[m_Index + 2] * 256 + m_pBuf[m_Index + 3];
		m_Index = m_Index + SegSize + 2;
	}

	// DRI(Define Restart Interval //
	if((m_pBuf[m_Index] == 0xff) && (m_pBuf[m_Index+1] == 0xdd))
	{
		WORD Lr = m_pBuf[m_Index+2]*256+m_pBuf[m_Index+3];
		Ri = m_pBuf[m_Index+4]*256+m_pBuf[m_Index+5];

		m_Index = m_Index + 6;
	}
}

void CJpeg::FindSOS()
{
	if((m_pBuf[m_Index] == 0xff)&&(m_pBuf[m_Index + 1] == 0xda))
	{
		int i;		
		WORD SegSize = m_pBuf[m_Index + 2] * 256 + m_pBuf[m_Index + 3];
		ScanHeader.Ns = m_pBuf[m_Index + 4];
		for(i=0; i<ScanHeader.Ns; i++)
		{
			ScanHeader.Cs[i] = m_pBuf[m_Index + 5 + i*2];
			ScanHeader.Td[i] = m_pBuf[m_Index + 6 + i*2] >> 4;
			ScanHeader.Ta[i] = m_pBuf[m_Index + 6 + i*2] & 0x0f;
		}

		ScanHeader.Ss = m_pBuf[m_Index + 5 + 2 * ScanHeader.Ns];
		ScanHeader.Se = m_pBuf[m_Index + 6 + 2 * ScanHeader.Ns];
		ScanHeader.Ah = m_pBuf[m_Index + 7 + 2 * ScanHeader.Ns] >> 4;
		ScanHeader.Al = m_pBuf[m_Index + 7 + 2 * ScanHeader.Ns] & 0x0f;

		m_Index = m_Index + SegSize + 2;

		// 최대 Sampling Factor 구함 //
		Hmax = Vmax = 0;
		for(i=0; i<FrameHeader.Nf; i++)
		{
			if(FrameHeader.H[i] > Hmax)
				Hmax = FrameHeader.H[i];
			if(FrameHeader.V[i] > Vmax)
				Vmax = FrameHeader.V[i];
		}
		
		// 실제 이미지 사이즈 저장 //
		m_rWidth = FrameHeader.X;
		m_rHeight = FrameHeader.Y;
		
		// 이미지 사이즈를 MCU 크기에 맞아 떨어지도록 다시 계산 //		
		if(FrameHeader.X % (8 * Hmax) != 0)
			FrameHeader.X = (FrameHeader.X / (8*Hmax) + 1)*(8*Hmax);
		if(FrameHeader.Y % (8 * Vmax) != 0)
			FrameHeader.Y = (FrameHeader.Y / (8*Vmax) + 1)*(8*Vmax);
		
		Decode();		
	}
}

WORD CJpeg::NextBit()
{	
	WORD Bit;
	BYTE B2;
	static BYTE B;	

	while(cnt == 0)
	{
		B = NextByte();
		cnt = 8;

		if(B == 0xff)
		{
			B2 = NextByte();
		}
	}

	Bit = B >> 7;
	cnt--;
	B = B << 1;

	return Bit;
}

BYTE CJpeg::NextByte()
{
	return *(pByte++);
}

BYTE CJpeg::hDecode(int Th)
{
	int i = 1, j;
	WORD CODE = NextBit();
	BYTE Value;	

	while((CODE > TbH[Th].MAXCODE[i])||(TbH[Th].MAXCODE[i] == 65535))
	{
		i++;
		CODE = (CODE << 1) + NextBit();
	}
	j = TbH[Th].VALPTR[i];
	j = j + CODE - TbH[Th].MINCODE[i];

	Value = TbH[Th].HUFFVAL[j];

	return Value;
}

WORD CJpeg::Receive(BYTE SSSS)
{
	BYTE i=0;
	WORD V = 0;
	while(i != SSSS)
	{
		i++;
		V = (V << 1) + NextBit();
	}
	return V;
}

short CJpeg::Extend(WORD V, BYTE T)
{
	WORD Vt = 1 << (T-1);
	if( V < Vt )
	{
		Vt = (-1 << T) + 1;
		V = V + Vt;
	}
	return (short)V;
}

void CJpeg::DecodeDC(int Th)
{
	BYTE T = hDecode(Th);
	ZZ[0] = Extend(Receive(T), T);
}

void CJpeg::DecodeAC(int Th)
{
	int k = 1;
	memset((LPSTR)&ZZ[1], 0, 63 * sizeof(short));
	BYTE RS, SSSS, RRRR, R;

	// RRRR : ZZ에서 0 이 아닌 전 값으로부터의 상대적인 위치
	// SSSS : 0이 아닌 값의 범위(category)
	

	while(TRUE)
	{
		RS = hDecode(Th);
		SSSS = RS % 16;
		RRRR = RS >> 4;
		R = RRRR;
		if(SSSS == 0)
		{
			if(R == 15)
				k = k + 16;
			else
				break;
		}
		else
		{
			k = k + R;
			ZZ[k] = Extend(Receive(SSSS), SSSS);
			if(k == 63)
				break;
			else
				k++;
		}
	}
}

void CJpeg::DecodeDU(int N) // N = Component ID 0/1/2
{	
	int i;
	short *pos;

	DecodeDC(ScanHeader.Td[N]);
	DecodeAC(ScanHeader.Ta[N] + 16);

	// Differential DC Restoration//
	ZZ[0] = ZZ[0] + PrevDC[N];
	PrevDC[N] = ZZ[0];	

	// Dequantization //
	pos = ZZ;
	for(i=0; i<64; i++)
	{
		*pos = *pos * TbQ[FrameHeader.Tq[N]].Q[i];
		pos++;
	}

	// Undo Zigzag Order //
	Zigzag();
	
	// Inverce Discrete Cosine Transform //
	IDCT();
	
	// Level Shifting  & Correct Error //	
	pos = ZZ;
	for(i=0; i<64; i++)
	{
		*pos = *pos + 128;
		
		if(*pos < 0)
			*pos = 0;
		else if(*pos > 255)
			*pos = 255;

		pos++;
	}	
	
}

void CJpeg::Zigzag()
{
	int Index[64] = 
	{0,  1,  5,  6,  14, 15, 27, 28,
	 2,  4,  7,  13, 16, 26, 29, 42, 
	 3,  8,  12, 17, 25, 30, 41, 43, 
	 9,  11, 18, 24, 31, 40, 44, 53, 
	 10, 19, 23, 32, 39, 45, 52, 54, 
	 20, 22, 33, 38, 46, 51, 55, 60,
	 21, 34, 37, 47, 50, 56, 59, 61,
	 35, 36, 48, 49, 57, 58, 62, 63};
	
	short Temp[64];
	memcpy(Temp, ZZ, 64 * sizeof(short));
	int i, j, idx;
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
		{
			idx = (i<<3)+j;
			ZZ[idx] = Temp[Index[idx]];
		}
}

void CJpeg::IDCT()
{
	int x, y, index, coeff, idx;
	float tmp1[8][8];
	float tmp2[8][8];
	float dct_coeff[8][8] =
	{
		+0.7071f, +0.7071f, +0.7071f, +0.7071f, +0.7071f, +0.7071f, +0.7071f, +0.7071f, 
		+0.9808f, +0.8315f, +0.5556f, +0.1951f, -0.1951f, -0.5556f, -0.8315f, -0.9808f,
		+0.9239f, +0.3827f, -0.3827f, -0.9239f, -0.9239f, -0.3827f, +0.3827f, +0.9239f,
		+0.8315f, -0.1951f, -0.9808f, -0.5556f, +0.5556f, +0.9808f, +0.1951f, -0.8315f,
		+0.7071f, -0.7071f, -0.7071f, +0.7071f, +0.7071f, -0.7071f, -0.7071f, +0.7071f,
		+0.5556f, -0.9808f, +0.1951f, +0.8315f, -0.8315f, -0.1951f, +0.9808f, -0.5556f,
		+0.3827f, -0.9239f, +0.9239f, -0.3827f, -0.3827f, +0.9239f, -0.9239f, +0.3827f,
		+0.1951f, -0.5556f, +0.8315f, -0.9808f, +0.9808f, -0.8315f, +0.5556f, -0.1951f
	};
	


	for(y=0; y<8; y++)
	{
		for(x=0; x<8; x++)
		{
			tmp2[y][x] = (float)ZZ[(y<<3)+x];
		}
	}

	for(x=0; x<8; x++) // 1-D column IDCT
	{
		for(coeff=0; coeff<8; coeff++)
		{
			tmp1[coeff][x] = 0.;
			for(index=0; index<8; index++)
			{
				tmp1[coeff][x] += tmp2[index][x] * dct_coeff[index][coeff];
			}			
		}		
	}	
	
	for(y=0; y<8; y++) // 1-D row IDCT
	{
		for(coeff=0; coeff<8; coeff++)
		{
			tmp2[y][coeff] = 0.;
			for(index=0; index<8; index++)
			{
				tmp2[y][coeff] += tmp1[y][index] * dct_coeff[index][coeff];
			}				
			idx = (y<<3)+coeff;			
			ZZ[idx] = (short)(tmp2[y][coeff]/4.);			
		}		
	}

}




void CJpeg::DecodeMCU(int mx, int my)
{
	int i, j, k, l, m, n, o;
	int Ns = ScanHeader.Ns;
	int H, V, Rh, Rv;
	int mWidth = Hmax * 8, mHeight = Vmax * 8;	
	int bWidth = FrameHeader.X * Ns, bHeight = FrameHeader.Y;
	int idx1, idx2, idx3;
	
	for(k=0; k<Ns; k++)
	{
		H = FrameHeader.H[k];
		V = FrameHeader.V[k];		
		Rh = Hmax / H;
		Rv = Vmax / V;

		for(l=0; l<V; l++)
		{
			for(m=0; m<H; m++)
			{
				DecodeDU(k);
				for(i=0; i<8; i++)
				{
					for(j=0; j<8; j++)
					{
						idx1 = ((l<<3)+i)*Rv;
						idx2 = ((m<<3)+j)*Rh;
						for(n = 0; n<Rv; n++)
						{
							for(o = 0; o<Rh; o++)
							{
								MCU[(idx1+n) * mWidth + (idx2+o)].C[k] = (BYTE)ZZ[(i<<3)+j];
							}
						}
					}
				}
			}		
		}
		
		for(i=0; i<mHeight; i++)
			for(j=0; j<mWidth; j++)
			{
				idx1 = my * mHeight + i;
				idx2 = mx * mWidth + j;
				idx3 = i * mWidth + j;
				for(n=0; n<Ns; n++)
					m_pData[(idx1)*bWidth+((idx2)*Ns + n)] = MCU[idx3].C[n];
			}
	}
}

void CJpeg::Decode()
{
	int Ns = ScanHeader.Ns;	
	int mx = FrameHeader.X / 8 / Hmax;
	int my = FrameHeader.Y / 8 / Vmax;	
	int i, j, k;		
	
	cnt = 0;	

	if(m_pData != NULL)
		delete [] m_pData;

	m_pData = new BYTE[FrameHeader.X * FrameHeader.Y * 3];

	for(i=0; i<Ns; i++)
		PrevDC[i] = 0;

	MCU = new SET[Vmax * Hmax * 64];

	// NextByte 함수를 위한 포인터 리셋 //
	pByte = &m_pBuf[m_Index];
	
	// 실질적인 Decoding Procedure //
	int Count = 0;///
	
	for(i=0; i<my; i++)
		for(j=0; j<mx; j++)
		{			
			DecodeMCU(j, i);
			Count++;
			if(Count == Ri) // Restart Marker Decode
			{
				Count = 0;
				if((*pByte == 0xff)&&((*(pByte+1) & 0xf0) == 0xd0))
				{
					cnt = 0;
					pByte = pByte + 2;
					for(k=0; k<3; k++)
						PrevDC[k] = 0;
				}				
			}
		}

	
	delete [] MCU;



	// RGB 로 바꿈 //
	ConvertYUV2RGB();

}

int CJpeg::GetWidth()
{
	return FrameHeader.X;
}

int CJpeg::GetHeight()
{
	return FrameHeader.Y;
}

void CJpeg::ConvertYUV2RGB()
{
	int i, j;
	int Width = GetWidth();
	int Height = GetHeight();
	int Size = Width * Height * ScanHeader.Ns;
	float Y, Cb, Cr;
	float R, G, B;
	BYTE *pTemp = m_pData;
	BYTE *pBuf = new BYTE[Size];
	BYTE *pos = pBuf;	
	
	memcpy(pos, pTemp, Size);

	for(i=0; i<Height; i++)
		for(j=0; j<Width; j++)
		{
			Y = *pos;pos++;
			
			if(ScanHeader.Ns == 3)
			{
				Cb = *pos;pos++;
				Cr = *pos;pos++;
				
				R = Y + 1.402f * (Cr-128);				
				G = Y - 0.34414f * (Cb-128) - 0.71414f * (Cr-128);
				B = Y + 1.772f * (Cb-128);				
				
				if(R>255) R = 255; if(R<0) R = 0;
				if(G>255) G = 255; if(G<0) G = 0;
				if(B>255) B = 255; if(B<0) B = 0;
				*pTemp = (BYTE)B;pTemp++;
				*pTemp = (BYTE)G;pTemp++;
				*pTemp = (BYTE)R;pTemp++;
			}
			else if(ScanHeader.Ns == 1)
			{
				*pTemp = (BYTE)Y;pTemp++;
				*pTemp = (BYTE)Y;pTemp++;
				*pTemp = (BYTE)Y;pTemp++;
			}
		}
	delete [] pBuf;

	// Clipping Useless Region //
	int RealBMPWidth = (m_rWidth * 3 + 3)/4 * 4;

	if((Width != m_rWidth)||(Height != m_rHeight))
	{
		BYTE *pBuf2 = new BYTE [RealBMPWidth  * m_rHeight];

		for(i=0; i<m_rHeight; i++)
		{
			memcpy(&pBuf2[i*RealBMPWidth], &m_pData[i*Width*3], RealBMPWidth);
		}

		delete [] m_pData;
		m_pData = pBuf2;

		// 본래의 이미지 사이즈로 복원 //
		FrameHeader.X = m_rWidth;
		FrameHeader.Y = m_rHeight;
	}


	// Flip for BMP Structure //
	BYTE *pLine = new BYTE[RealBMPWidth];
	for(i=0; i<m_rHeight/2; i++)
	{
		pTemp = &m_pData[i*RealBMPWidth];
		memcpy(pLine, pTemp, RealBMPWidth);
		memcpy(pTemp, &m_pData[(m_rHeight - 1 - i)*RealBMPWidth], RealBMPWidth);
		memcpy(&m_pData[(m_rHeight - 1 - i)*RealBMPWidth], pLine, RealBMPWidth);
	}
	delete [] pLine;	


}

void CJpeg::SaveJPG(LPCSTR FileName, int Width, int Height, BYTE *pp)
{
	m_rWidth = Width;
	m_rHeight = Height;	
	
	int BMPWidth = (Width * 3 + 3)/4*4; // BITMAP의 4의 배수인 넓이
	int bWidth = Width;
	int bHeight = Height;

	if(Width % 8 != 0)
		bWidth = (Width/8 + 1)*8; // bWidth 는 8의 배수로 만든 넓이
	if(Height % 8 != 0)
		bHeight = (Height/8 + 1)*8; // bHeight 는 8의 배수로 만든 높이

	int i, j, k;

	// Huffman Table 초기화 //
	for(i=0; i<20; i++)
	{
		if(TbH[i].Flag)
		{
			delete [] TbH[i].HUFFCODE;
			delete [] TbH[i].HUFFSIZE;
			delete [] TbH[i].HUFFVAL;
		}	
	}
	
	DWORD dwWritten;
	HANDLE fh = CreateFile(FileName, GENERIC_READ|GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
		return;
	PutSOI(fh);						// Start Of Image //
	PutDQT(fh);						// Quantization Table 저장 //
	PutDHT(fh);						// Huffman Table 저장 //
	PutSOF(fh, Width, Height);		// FrameHeader 저장 //
	PutSOS(fh);						// Scan Header 저장 //


	// 버퍼를 마련합시다! //
	if(m_pData != NULL)
		delete [] m_pData;	
	m_pData = new BYTE[(bWidth * 3) * bHeight];

	memset(m_pData, 0, (bWidth * 3) * bHeight); // 0으로 초기화 //

	// 8의 배수에 맞도록 버퍼를 만들어서 원래 이미지를 복사합니다 //
	for(i=0; i<Height; i++)
		memcpy(&m_pData[i*(bWidth*3)], &pp[i*BMPWidth], BMPWidth);

	// RGB Color를 YCbCr Color로 변환합니다. //
	float R, G, B;
	float y, cb, cr;
	BYTE *pos;	

	for(i=0; i<Height; i++)
	{
		pos = &m_pData[i*(bWidth*3)];
		for(j=0; j<Width; j++)
		{
			B = (float)*pos;
			G = (float)*(pos+1);
			R = (float)*(pos+2);

			y = 0.299f * R + 0.587f * G + 0.114f * B;
			cb = -0.1687f * R - 0.3313f * G + 0.5f * B + 128;
			cr = 0.5f*R - 0.4187f*G - 0.0813f*B + 128;			
			
			/*y = (77 * R + 150 * G + 29 * B)>>8;
			cb = (B - y) + 128;
			cr = (R - y) + 128;*/

			if(y>255.) y = 255.; if(y<0.) y = 0.;
			if(cb>255.) cb = 255.; if(cb<0.) cb = 0.;
			if(cr>255.) cr = 255.; if(cr<0.) cr = 0.;			

			*pos = (BYTE)y;
			*(pos+1) = (BYTE)cb;
			*(pos+2) = (BYTE)cr;

			pos = pos + 3;
		}
	}

	
	// Y, Cb, Cr Plane Buffer //
	Y = new short[bWidth * bHeight];
	memset(Y, 0, sizeof(short) * bWidth * bHeight);
	Cb = new short[bWidth * bHeight];
	memset(Cb, 0, sizeof(short) * bWidth * bHeight);
	Cr = new short[bWidth * bHeight];
	memset(Cr, 0, sizeof(short) * bWidth * bHeight);

	int idx1, idx2;

	
	// Level Shifting -128 //
	for(i=0; i<Height; i++)
	{
		for(j=0; j<Width; j++)
		{
			idx1 = i*bWidth+j;
			idx2 = i*(bWidth*3)+j*3;
			Y[idx1] = m_pData[idx2] - 128;
			Cb[idx1] = m_pData[idx2+1] - 128;
			Cr[idx1] = m_pData[idx2+2] - 128;
		}
	}
	

	// DCT & Zigzag & Quantization //
	for(i=0; i<bHeight; i = i + 8)
	{
		for(j=0; j<bWidth; j = j + 8)
		{
			DCT(&Y[i*bWidth+j], bWidth, FALSE);
			DCT(&Cb[i*bWidth+j], bWidth, TRUE);
			DCT(&Cr[i*bWidth+j], bWidth, TRUE);
		}
	}

	// Differential DC Process //

	int Num_X = bWidth/8;
	int Num_Y = bHeight/8;
	int idx = 0;

	short *DC1 = new short[Num_X * Num_Y];
	short *DC2 = new short[Num_X * Num_Y];
	short *DC3 = new short[Num_X * Num_Y];

	for(i=0; i<bHeight; i = i + 8)
	{
		for(j=0; j<bWidth; j = j + 8)
		{
			DC1[idx] = Y[i*bWidth+j];				
			DC2[idx] = Cb[i*bWidth+j];
			DC3[idx] = Cr[i*bWidth+j];			
			idx++;
		}
	}	

	for(i=Num_X * Num_Y - 1; i>0; i--)
	{		
		DC1[i] = DC1[i] - DC1[i-1];
		DC2[i] = DC2[i] - DC2[i-1];
		DC3[i] = DC3[i] - DC3[i-1];	
	}

	idx = 0;
	for(i=0; i<bHeight; i = i + 8)
	{
		for(j=0; j<bWidth; j = j + 8)
		{
			 Y[i*bWidth+j] = DC1[idx];
			 Cb[i*bWidth+j] = DC2[idx];
			 Cr[i*bWidth+j] = DC3[idx];
			idx++;
		}
	}	

	delete [] DC1;
	delete [] DC2;
	delete [] DC3;	

	/* AC 허프만 테이블 검색을 빠르게 하기 위하여
	   허프만 테이블 구조체에 있는 *PT를 설정한다. */


	int Num, iTh[4] = {16, 17}, Th, key;

	for(i=0; i<2; i++)
	{
		Th = iTh[i];
		Num = TbH[Th].Num;
		TbH[Th].PT = new int[251];		

		for(k=0; k<251; k++)
		{
			key = -9999;
			
			for(j=0; j<Num; j++)
			{
				if(TbH[Th].HUFFVAL[j] == k)
				{
					key = j;
					break;
				}				
			}
			TbH[Th].PT[k] = key;
		}
	}

	/* HUFFMAN CODE ENCODEING!! */
	 
	m_pBuf = new BYTE[bWidth * bHeight * 3]; // 실제로 인코딩된 데이터가 저장될 버퍼
	m_Index = 0; // 인덱스 리셋
	cnt = 0; // Bit총 리셋 
	
	hEncode(bWidth, bHeight);

	while(cnt != 0)
		ShotBit(0);

	WriteFile(fh,(LPSTR)m_pBuf,m_Index,&dwWritten,NULL);

	delete [] m_pBuf;

	PutEOI(fh);
	
	
	// 임시 메모리 모두 해제 //

	for(i=0; i<2; i++)
	{
		Th = iTh[i];
		delete [] TbH[Th].PT;
	}	

	for(i=0; i<bHeight; i++)
		for(j=0; j<bWidth; j++)
		{
			idx1 = i*bWidth*3 + j*3;
			m_pData[idx1] = (BYTE)Y[i*bWidth+j];
			m_pData[idx1+1] = (BYTE)Cb[i*bWidth+j];
			m_pData[idx1+2] = (BYTE)Cr[i*bWidth+j];
		}	
	
	delete [] Y;
	delete [] Cb;
	delete [] Cr;

	// 파일 폐쇄 //

	CloseHandle(fh);
	
}

void CJpeg::PutSOI(HANDLE hFile)
{
	// SOI 저장 //
	DWORD dwWritten;
	WORD Marker = (0xd8 << 8) | 0xff;
	WriteFile(hFile,(LPSTR)&Marker,2,&dwWritten,NULL);
}

void CJpeg::PutSOF(HANDLE hFile, int Width, int Height)
{
	WORD Marker, SegSize, w;
	BYTE c;
	int i;

	DWORD dwWritten;

	Marker = (0xc0 << 8) | 0xff;
	WriteFile(hFile,(LPSTR)&Marker,2,&dwWritten,NULL);

	SegSize = 17; SegSize = (SegSize << 8) | (SegSize >> 8);
	WriteFile(hFile,(LPSTR)&SegSize,2,&dwWritten,NULL);
	
	c = 8; 
	WriteFile(hFile,(LPSTR)&c,1,&dwWritten,NULL);

	
	w = (WORD)Height; w = (w << 8) | (w >> 8);
	WriteFile(hFile,(LPSTR)&w,2,&dwWritten,NULL);

	w = (WORD)Width; w = (w << 8) | (w >> 8);
	WriteFile(hFile,(LPSTR)&w,2,&dwWritten,NULL);

	c = 3; 
	WriteFile(hFile,(LPSTR)&c,1,&dwWritten,NULL);
	for(i=1; i<=3; i++)
	{
		c = (BYTE)i; // Component Identifier
		WriteFile(hFile,(LPSTR)&c,1,&dwWritten,NULL);

		c = 17;    // HiVi
		WriteFile(hFile,(LPSTR)&c,1,&dwWritten,NULL);
		
		if(i==1)
			c=0;
		else
			c=1;
		   // Tqi
		WriteFile(hFile,(LPSTR)&c,1,&dwWritten,NULL);
	}
}

void CJpeg::PutSOS(HANDLE hFile)
{
	WORD Marker, SegSize;
	BYTE c;
	int i;
	DWORD dwWritten;

	Marker = (0xda << 8) | 0xff;
	WriteFile(hFile,(LPSTR)&Marker,2,&dwWritten,NULL);

	SegSize = 12; SegSize = (SegSize << 8) | (SegSize >> 8);
	// Segment Size
	WriteFile(hFile,(LPSTR)&SegSize,2,&dwWritten,NULL);

	c = 3; // Ns = 3
	
	WriteFile(hFile,(LPSTR)&c,1,&dwWritten,NULL);

	for(i=1; i<=3; i++)
	{
		c = (BYTE)i; // Scan Component Selector		
		WriteFile(hFile,(LPSTR)&c,1,&dwWritten,NULL);
		if(i==1)
			c=0;
		else
			c=0x11;
		// TdTa
		WriteFile(hFile,(LPSTR)&c,1,&dwWritten,NULL);

	}
	c = 0;  // Ss
	WriteFile(hFile,(LPSTR)&c,1,&dwWritten,NULL);
	c = 0x3f; // Se
	WriteFile(hFile,(LPSTR)&c,1,&dwWritten,NULL);
	c = 0; // AhAl
	WriteFile(hFile,(LPSTR)&c,1,&dwWritten,NULL);
}

void CJpeg::PutDQT(HANDLE hFile)
{
	WORD Marker, SegSize;
	BYTE c;
	DWORD dwWritten;
	// Luminance Quantization Table //
	BYTE Qtb0[64] = {16, 11, 12, 14, 12, 10, 16, 14,
					13, 14, 18, 17, 16, 19, 24, 40,
					26, 24, 22, 22, 24, 49, 36, 37,
					29, 40, 58, 51, 61, 60, 57, 51,
					56, 55, 64, 72, 92, 78, 64, 68,
					87, 69, 66, 57, 80, 109, 81, 87,
					95, 98, 103, 104, 103, 62, 77, 113,
					121, 112, 100, 120, 92, 101, 103, 99};
	// Chrominance Quantization Table //
	BYTE Qtb1[64] = {17, 18, 18, 24, 21, 24, 47, 26,
					26, 47, 99, 66, 56, 66, 99, 99,
					99 ,99, 99, 99, 99, 99, 99, 99, 
					99, 99, 99, 99, 99, 99, 99, 99, 
					99, 99, 99, 99, 99, 99, 99, 99, 
					99, 99, 99, 99, 99, 99, 99, 99, 
					99, 99, 99, 99, 99, 99, 99, 99, 
					99, 99, 99, 99, 99, 99, 99, 99	};

	Marker = (0xdb << 8) | 0xff;
	// Marker
	WriteFile(hFile,(LPSTR)&Marker,2,&dwWritten,NULL);
	
	SegSize = 67; SegSize = (SegSize << 8) | (SegSize >> 8);
	// Segment Size
	WriteFile(hFile,(LPSTR)&SegSize,2,&dwWritten,NULL);

	
	c = 0; // PqTq = 0
	WriteFile(hFile,(LPSTR)&c,1,&dwWritten,NULL);
	// Q0 ~ Q63
	WriteFile(hFile,(LPSTR)&Qtb0,64,&dwWritten,NULL);

	
	Marker = (0xdb << 8) | 0xff;
	// Marker
	WriteFile(hFile,(LPSTR)&Marker,2,&dwWritten,NULL);
	
	SegSize = 67; SegSize = (SegSize << 8) | (SegSize >> 8);
	// Segment Size
	WriteFile(hFile,(LPSTR)&SegSize,2,&dwWritten,NULL);
	
	c = 1; //_lwrite(hFile, (LPSTR)&c, 1); // PqTq = 1
	WriteFile(hFile,(LPSTR)&c,1,&dwWritten,NULL);
	//_lwrite(hFile, (LPSTR)Qtb1, 64); // Q0 ~ Q63
	WriteFile(hFile,(LPSTR)&Qtb1,64,&dwWritten,NULL);
}

void CJpeg::DCT(short * pos, int bWidth, BOOL Flag)
{
	// DCT 분만 아니라, DCT 후의 Zigzag 까지... 게다가 Quantization 까지!?//
	
	BYTE Qtb0[64] ={16, 11, 12, 14, 12, 10, 16, 14, 
					13, 14, 18, 17, 16, 19, 24, 40, 
					26, 24, 22, 22, 24, 49, 36, 37, 
					29, 40, 58, 51, 61, 60, 57, 51, 
					56, 55, 64, 72, 92, 78, 64, 68, 
					87, 69, 66, 57, 80, 109, 81, 87,
					95, 98, 103, 104, 103, 62, 77, 113,
					121, 112, 100, 120, 92, 101, 103, 99};
	
	BYTE Qtb1[64] ={17, 18, 18, 24, 21, 24, 47, 26,
					26, 47, 99, 66, 56, 66, 99, 99,
					99 ,99, 99, 99, 99, 99, 99, 99, 
					99, 99, 99, 99, 99, 99, 99, 99, 
					99, 99, 99, 99, 99, 99, 99, 99, 
					99, 99, 99, 99, 99, 99, 99, 99, 
					99, 99, 99, 99, 99, 99, 99, 99, 
					99, 99, 99, 99, 99, 99, 99, 99	};


	int i, j;
	int x, y, u, v;	
	float Cu, Cv;
	float Sum;
	float dct_coeff[8][8] =
	{
		   +1.0f,	 +1.0f,    +1.0f,	 +1.0f,	   +1.0f,	 +1.0f,	   +1.0f,	 +1.0f, 
		+0.9808f, +0.8315f, +0.5556f, +0.1951f, -0.1951f, -0.5556f, -0.8315f, -0.9808f,
		+0.9239f, +0.3827f, -0.3827f, -0.9239f, -0.9239f, -0.3827f, +0.3827f, +0.9239f,
		+0.8315f, -0.1951f, -0.9808f, -0.5556f, +0.5556f, +0.9808f, +0.1951f, -0.8315f,
		+0.7071f, -0.7071f, -0.7071f, +0.7071f, +0.7071f, -0.7071f, -0.7071f, +0.7071f,
		+0.5556f, -0.9808f, +0.1951f, +0.8315f, -0.8315f, -0.1951f, +0.9808f, -0.5556f,
		+0.3827f, -0.9239f, +0.9239f, -0.3827f, -0.3827f, +0.9239f, -0.9239f, +0.3827f,
		+0.1951f, -0.5556f, +0.8315f, -0.9808f, +0.9808f, -0.8315f, +0.5556f, -0.1951f
	};

	for(v=0; v<8; v++)
	{
		for(u=0; u<8; u++)
		{
			Sum = 0;

			for(y=0; y<8; y++)
				for(x=0; x<8; x++)
					Sum = Sum + pos[(int)(y*bWidth+x)] * dct_coeff[u][x] * dct_coeff[v][y];
			
			Cu = 1.; if(u == 0) Cu = 0.7071f;
			Cv = 1.; if(v == 0) Cv = 0.7071f;
			ZZ[(int)(v*8+u)] = (short)(Cu * Cv * Sum / 4.);			
		}
	}	
	
	Zigzag2();	

	if(Flag) // TRUE : Chrominance, FALSE : Luminance
		for(i=0; i<64; i++)
			ZZ[i] = ZZ[i] / Qtb1[i];
	else
		for(i=0; i<64; i++)
			ZZ[i] = ZZ[i] / Qtb0[i];
			


	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
			pos[i*bWidth+j] = ZZ[i*8+j];
	
}

void CJpeg::Zigzag2()
{
	int Index[64] = 
	{0,  1,  5,  6,  14, 15, 27, 28,
	 2,  4,  7,  13, 16, 26, 29, 42, 
	 3,  8,  12, 17, 25, 30, 41, 43, 
	 9,  11, 18, 24, 31, 40, 44, 53, 
	 10, 19, 23, 32, 39, 45, 52, 54, 
	 20, 22, 33, 38, 46, 51, 55, 60,
	 21, 34, 37, 47, 50, 56, 59, 61,
	 35, 36, 48, 49, 57, 58, 62, 63};
	
	short Temp[64];
	memcpy(Temp, ZZ, 64 * sizeof(short));
	int i, j, idx;
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
		{
			idx = (i<<3)+j;
			ZZ[Index[idx]] = Temp[idx];
		}

}

void CJpeg::PutDHT(HANDLE hFile)
{
	/* standard 허프만 테이블을 읽어들여서 멤버 구조체에 설정한
	   후 저장하고자 하는 파일에 적어넣는 함수입니다. */	
	
	m_pBuf = new BYTE[421];	
	BYTE HuffTb[421] = {255, 196, 1, 162, 0, 0, 1, 5, 1, 1, 1, 
						1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 
						3, 4, 5, 6, 7, 8, 9, 10, 11, 16, 0, 2, 
						1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1,
						125, 1, 2, 3, 0, 4, 17, 5, 18, 33, 49,
						65, 6, 19, 81, 97, 7, 34, 113, 20, 50,
						129, 145, 161, 8, 35, 66, 177, 193, 21, 
						82, 209, 240, 36, 51, 98, 114, 130, 9, 
						10, 22, 23, 24, 25, 26, 37, 38, 39, 40, 
						41, 42, 52, 53, 54, 55, 56, 57, 58, 67,
						68, 69, 70, 71, 72, 73, 74, 83, 84, 85, 
						86, 87, 88, 89, 90, 99, 100, 101, 102, 
						103, 104, 105, 106, 115, 116, 117, 118, 
						119, 120, 121, 122, 131, 132, 133, 134,
						135, 136, 137, 138, 146, 147, 148, 149, 
						150, 151, 152, 153, 154, 162, 163, 164, 
						165, 166, 167, 168, 169, 170, 178, 179,
						180, 181, 182, 183, 184, 185, 186, 194, 
						195, 196, 197, 198, 199, 200, 201, 202, 
						210, 211, 212, 213, 214, 215, 216, 217, 
						218, 225, 226, 227, 228, 229, 230, 231, 
						232, 233, 234, 241, 242, 243, 244, 245, 
						246, 247, 248, 249, 250, 1, 0, 3, 1, 1,
						1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 
						1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 17, 
						0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 
						1, 2, 119, 0, 1, 2, 3, 17, 4, 5, 33, 49,
						6, 18, 65, 81, 7, 97, 113, 19, 34, 50, 
						129, 8, 20, 66, 145, 161, 177, 193, 9, 
						35, 51, 82, 240, 21, 98, 114, 209, 10, 
						22, 36, 52, 225, 37, 241, 23, 24, 25, 26, 
						38, 39, 40, 41, 42, 53, 54, 55, 56, 57, 
						58, 67, 68, 69, 70, 71, 72, 73, 74, 83, 
						84, 85, 86, 87, 88, 89, 90, 99, 100, 101, 
						102, 103, 104, 105, 106, 115, 116, 117, 
						118, 119, 120, 121, 122, 130, 131, 132, 
						133, 134, 135, 136, 137, 138, 146, 147, 
						148, 149, 150, 151, 152, 153, 154, 162, 
						163, 164, 165, 166, 167, 168, 169, 170, 
						178, 179, 180, 181, 182, 183, 184, 185, 
						186, 194, 195, 196, 197, 198, 199, 200, 
						201, 202, 210, 211, 212, 213, 214, 215, 
						216, 217, 218, 226, 227, 228, 229, 230, 
						231, 232, 233, 234, 242, 243, 244, 245, 
						246, 247, 248, 249, 250, 255};
	memcpy(m_pBuf, HuffTb, 421);
	m_Index = 0; // Huffman Table을 읽어들이기 위하여 Index를 Reset
	FindDHT();
	//_lwrite(hFile, (LPSTR)m_pBuf, 420);
	DWORD	dwWritten;
	WriteFile(hFile,(LPSTR)m_pBuf,420,&dwWritten,NULL);
	delete [] m_pBuf;
}

// 주어진 값이 어느 케티고리에 소속되는 지를 반환하는 함수 //
BYTE CJpeg::GetCategory(short V)
{	
	BYTE Num = 0;
	if(V < 0)
		V = -V;
	while(V != 0)
	{
		V = V >> 1;
		Num++;
	}
	return Num;	
}


void CJpeg::hEncode(int bWidth, int bHeight)
{
	int i, j;

	for(i=0; i<bHeight; i = i + 8)
	{
		for(j=0; j<bWidth; j = j + 8)
		{
			EncodeDU(&Y[i*bWidth+j], FALSE, bWidth);
			EncodeDU(&Cb[i*bWidth+j], TRUE, bWidth);
			EncodeDU(&Cr[i*bWidth+j], TRUE, bWidth);
		}
	}

}

void CJpeg::EncodeDU(short * pos, BOOL Flag, int bWidth)
{
	WORD Code;
	BYTE Val, Size;	
	BYTE SSSS, Rs;	
	short XX[64], DIFF;	
	BYTE ThDC, ThAC;
	int idx = 0;	
	int i, j;
	
	ThDC = 0; ThAC = 16;
	if(Flag)
	{
		ThDC = 1; ThAC = 17;
	}	

	for(i=0; i<8; i++)
	{
		for(j=0; j<8; j++)
		{
			XX[idx] = pos[i*bWidth+j];
			idx++;
		}
	}

	// DC Encode //
	DIFF = XX[0];
	Val = GetCategory(DIFF);
	Code = TbH[ThDC].HUFFCODE[Val];
	Size = TbH[ThDC].HUFFSIZE[Val];
	if(DIFF < 0)
		DIFF--;	
	ChargeCode(Code, Size);
	ChargeCode((WORD)DIFF, Val);

	// AC Encode //	
	BYTE k = 0, R = 0;	
	while(TRUE)
	{
		k++;
		if(XX[k] == 0)
		{
			if(k==63)
			{
				Rs = 0x00;
				Code = TbH[ThAC].HUFFCODE[TbH[ThAC].PT[Rs]];
				Size = TbH[ThAC].HUFFSIZE[TbH[ThAC].PT[Rs]];
				ChargeCode((WORD)Code, Size);
				break;
			}
			else
				R++;
		}
		else
		{
			while(R>15)
			{
				Rs = 0xf0;
				Code = TbH[ThAC].HUFFCODE[TbH[ThAC].PT[Rs]];
				Size = TbH[ThAC].HUFFSIZE[TbH[ThAC].PT[Rs]];
				ChargeCode((WORD)Code, Size);
				R = R - 16;
			}
			
			SSSS = GetCategory(XX[k]);
			//Rs = (R << 4) | SSSS;
			Rs = (R * 16) + SSSS;
			Code = TbH[ThAC].HUFFCODE[TbH[ThAC].PT[Rs]];
			Size = TbH[ThAC].HUFFSIZE[TbH[ThAC].PT[Rs]];
			ChargeCode((WORD)Code, Size);
			DIFF = XX[k];
			if(DIFF < 0)
				DIFF = DIFF - 1;
			ChargeCode((WORD)DIFF, SSSS);
			R = 0;

			if(k==63)
				break;
		}
	}
	
	
}

void CJpeg::ChargeCode(WORD Code, int Size)
{
	int i;
	BYTE Bit;	

	for(i=0; i<Size; i++)
	{
		Bit = (Code >> (Size - 1 - i)) & 0x01;
		ShotBit(Bit);		
	}
}

void CJpeg::ShotBit(BYTE Bit)
{
	static BYTE Bullet = 0;

	Bit = Bit << (7-cnt);

	Bullet = Bullet | Bit;	
	
	cnt++;

	if(cnt == 8)
	{
		cnt = 0;
		m_pBuf[m_Index] = Bullet;
		m_Index++;
		if(Bullet == 0xff) // 0xff가 나오면 0x00으로 Byte Stuffing!!
		{
			m_pBuf[m_Index] = 0x00;
			m_Index++;
		}
		Bullet = 0;
	}
}

void CJpeg::PutEOI(HANDLE hFile)
{
	// EOI 저장 //
	WORD Marker = (0xd9 << 8) | 0xff;
	DWORD	dwWritten;
	WriteFile(hFile,(LPSTR)&Marker,2,&dwWritten,NULL);
	//_lwrite(hFile, (LPSTR)&Marker, 2);	
}
