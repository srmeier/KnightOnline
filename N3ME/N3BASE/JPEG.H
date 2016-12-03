//////////////////////////////////////////////////////////////////////
//
// Jpeg.h: interface for the CJpeg class.
//
// 주요동작 : JPEG 파일을 디코드하여 읽어들이거나 디스크에 쓴다.
//
// 원저자 : 모름
// 변경자 : 이현석
// E-Mail : evaasuka1004@hotmail.com
//////////////////////////////////////////////////////////////////////

#ifndef _JPEG_MODULE_BODY_
#define _JPEG_MODULE_BODY_

struct SET
{
	BYTE C[3]; // Y, Cb, Cr 성분
};

struct DQT
{
	BYTE Q[64]; // Qantization Table 값
};

struct DHT
{
	BOOL Flag; // 사용되었는지 여부를 나타내는 플래그
	int Num; // 허프만 코드의 수
	WORD *HUFFCODE; // 허프만 코드
	BYTE *HUFFSIZE; // 허프만 코드의 길이
	BYTE *HUFFVAL;  // 허프만 코드가 나타내는 값
	WORD MAXCODE[17]; // 해당 길이에서 가장 큰 코드
	WORD MINCODE[17]; // 해당 길이에서 가장 작은 코드
	int VALPTR[17]; // 해당 길이의 코드가 시작되는 인덱스
	int *PT; // VALUE로 INDEX를 빠르게 찾기 위한 포인터
};

struct FRAMEHEADER
{
	WORD Y; // 이미지의 높이
	WORD X; // 이미지의 넓이
	BYTE Nf; // 컴포넌트 수
	BYTE C[3]; // 컴포넌트 아이디
	BYTE H[3]; // 컴포넌트의 Horizontal Sampling Factor
	BYTE V[3]; // 컴포넌트의 Vertical Sampling Factor
	BYTE Tq[3]; // 해당 컴포넌트에 사용되는 양자화테이블 번호
};

struct SCANHEADER
{
	BYTE Ns; // 컴포넌트 수
	BYTE Cs[3]; // 컴포넌트 아이디
	BYTE Td[3]; // 컴포넌트의 DC Huffman Table 번호
	BYTE Ta[3]; // 컴포넌트의 AC Huffman Table 번호
	BYTE Ss;
	BYTE Se;
	BYTE Ah;
	BYTE Al;
};


class CJpeg  
{
public:

	// JPEG File을 Load하기 위한 함수들 //
	void LoadJPG(LPCSTR FileName); // JPEG File을 Load하는 함수	
	void FindSOI(); // Start of Image 마커를 찾는 함수
	void FindDQT(); // Quantization Table을 찾아 구조체에 설정하는 함수
	void FindDHT();	// Huffman Table을 찾아 구조체에 설정하는 함수
	void FindSOF(); // Frame Header를 찾아 구조체에 설정하는 함수
	void FindSOS(); // Scan Header를 찾아 구조체에 설정하는 함수
	void FindETC(); // DRI(Define Restart Interval) 로드 
	void Decode();  // 디코드를 위한 정보를 설정하고 디코드를 시작
	void DecodeMCU(int mx, int my); // MCU블럭을 디코드하는 함수
	void DecodeDU(int N); // 8x8 Data Unit를 디코드하는 함수
	void IDCT(); // Inverse DCT를 하는 함수
	void Zigzag(); // Zigzag순으로 되어있는 DU를 원상복귀시키는 함수
	void DecodeAC(int Th); // DU중, AC성분을 디코드하는 함수
	void DecodeDC(int Th); // DU중, DC성분을 디코드하는 함수
	short Extend(WORD V, BYTE T); // V를 카테고리 T에 맞도록 확장
	WORD Receive(BYTE SSSS); // 버퍼에서 SSSS비트만큼 읽어오는 함수
	BYTE hDecode(int Th); // 허프만 부호를 디코드하는 부분
	BYTE NextByte(); // 버퍼에서 다음 1 바이트를 읽어오는 함수
	WORD NextBit(); // 버퍼에서 다음 1 비트를 읽어오는 함수
	void ConvertYUV2RGB(); // 디코드된 데이터를 컬러모델을 바꿈과 동시에
						   // 비트맵에 호환되도록 변환하는 함수
	
	
	// JPEG File을 Save하기 위한 함수들 //	
	void SaveJPG(LPCSTR FileName, int Width, int Height, BYTE *pp); // JPEG 파일을 저장하는 함수

	void PutSOI(HANDLE hFile); // Start of Image 마커를 삽입
	void PutDQT(HANDLE hFile); // Quantizatino Table을 삽입
	void PutDHT(HANDLE hFile); // Huffmann Table을 삽입
	void PutSOF(HANDLE hFile, int Width, int Height); // FrameHeader를 삽입
	void PutSOS(HANDLE hFile); // ScanHeader를 삽입
	void PutEOI(HANDLE hFile); // End of Image 마커를 삽입

	void ShotBit(BYTE Bit); // 1비트를 버퍼에 저장하는 함수
	void ChargeCode(WORD Code, int Size ); // Size만큼의 길이(아랫자리로부터)로 Code를 버퍼에 저장하는 함수
	void EncodeDU(short *pos, BOOL Flag, int bWidth); // 8x8 Block을 인코드하는 함수
	void hEncode(int bWidth, int bHeight); // 허프만 인코딩 하는 부분
	BYTE GetCategory(short V); // 주어진 값의 카테고리를 구하는 함수	
	void Zigzag2(); // Zigzag순서로 만드는 함수
	void DCT(short *pos, int bWidth, BOOL Flag); // DCT를 한 후 Zigzag 및 Quantization 하는 함수

	int GetHeight(); // 이미지의 높이를 반환하는 함수
	int GetWidth();	// 이미지의 넓이를 반환하는 함수
	
	WORD Ri; // Restart Interval
	int m_rWidth; // 이미지의 실제적인 넓이
	int m_rHeight; // 이미지의 실제적인 높이
	BYTE * pByte; // NextByte()함수에서 쓰임
	
	int cnt; // 비트단위로 연산할 때 쓰이는 카운터
	
	short *Y;  // Save할 때 쓰이는 Y Buffer
	short *Cb; // Save할 때 쓰이는 Cb Buffer
	short *Cr; // Save할 때 쓰이는 Cr Buffer

	SET *MCU; // MCU 단위의 블럭
	
	BYTE Hmax; // Maximum Horizontal Sampling Factor
	BYTE Vmax; // Maximum Vertical Sampling Factor
	
	BYTE * m_pData;	// 이미지 버퍼
	BYTE * m_pBuf; // 버퍼
	int m_Index; // 버퍼의 위치를 나타내는 인덱스
	DQT TbQ[20]; // Quantization Table
	DHT TbH[20]; // Huffman Table
	short ZZ[64]; // 8x8 Block 정보를 담는 배열
	FRAMEHEADER FrameHeader; // FrameHeader 구조채
	SCANHEADER ScanHeader; // ScanHeader 구조체
	short PrevDC[3]; // DC 성분의 Predictor

	
	CJpeg();
	virtual ~CJpeg();

};

#endif //_JPEG_MODULE_BODY_