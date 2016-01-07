///////////////////////////////////////////////////////////////////////////////
/* ENCYPTION MAIN 
/* AND ITS API                                */
/* THIS PROGRAM IS PROGRAMMED FOR WIZGATE     */
/* BUT ALL COPYRIGHT RESERVED BY JIYOON CHUNG */
/* (c) ABLEX AND OHSUNGINC 2001.6.1           */
/* Classed by ks2000 2001.6.1				  */	
#ifndef __CJVCRYPTION_H
#define __CJVCRYPTION_H

typedef unsigned char 	T_OCTET;
typedef _int64			T_KEY;


class CJvCryption
{
private:
	T_KEY m_public_key;
	T_KEY m_private_key;
	T_KEY m_tkey;

public:
	CJvCryption();
	~CJvCryption();

	void SetPublicKey(T_KEY pk);
	void SetPrivateKey(T_KEY pk);

	void Init();

	void JvEncryption(T_KEY private_key, int len, T_OCTET *datain, T_OCTET *dataout);
	void JvDecryption(T_KEY private_key, int len, T_OCTET *datain, T_OCTET *dataout);

	void JvEncryption( int len, T_OCTET *datain, T_OCTET *dataout );
	void JvDecryption( int len, T_OCTET *datain, T_OCTET *dataout );
	void JvEncryptionFast( int len, T_OCTET *datain, T_OCTET *dataout );
	void JvDecryptionFast( int len, T_OCTET *datain, T_OCTET *dataout );

};

#endif
