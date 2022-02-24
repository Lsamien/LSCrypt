//aes.h

#pragma once

#include <windows.h>


extern "C"
{
#include "rijndael-alg-fst.h"
}

typedef struct _aes_Block
{
	unsigned __int64	b1;
	unsigned __int64	b2;
}AES_BLOCK, *PAES_BLOCK;


class CAes
{
public:
	enum aesDirection {edUnkown, edEncrypt, edDecrypt};
	enum aesKeyLen {KLUnkown, KL128, KL192, KL256};
private:
	int		m_nRounds;			//Number of encrypt round
	DWORD	*m_pRoundKeys;		// 4*10 = 40x32 bits. 128-bit keys use 10-rounds.
	enum aesDirection m_dir;	//encrypt or decrypt;
	enum aesKeyLen	m_keyLen;	//key length
	bool	m_bOK;
public:
	CAes();
	~CAes();
	void Init(const char *key, aesDirection dir, aesKeyLen keyLen = KL128);
	void Init(const wchar_t *key, aesDirection dir, aesKeyLen keyLen = KL128);
	aesDirection GetDir(void) { return m_dir; }
	aesKeyLen GetKeyLen(void) { return m_keyLen; }
	bool BurnBlock(AES_BLOCK *input, AES_BLOCK *output, DWORD nBlocks);
	static DWORD GetMinMultiAesBlock(DWORD len);
};




