//aes.cpp
#include "aes.h"


CAes::CAes()
{
	m_pRoundKeys = NULL;
	m_bOK = false;
	m_dir = edUnkown;
	m_keyLen = KLUnkown;
}


CAes::~CAes()
{
	delete [] m_pRoundKeys;
	m_pRoundKeys = NULL;
	
}



void CAes::Init(const char *key, CAes::aesDirection dir, CAes::aesKeyLen keyLen)
{
	delete [] m_pRoundKeys;
	m_pRoundKeys = NULL;

	m_bOK = false;

	if(dir != edEncrypt && dir != edDecrypt) return;
	if(!key) return;
	
	if(keyLen == KL128)
	{
		//only support 128bit
		m_nRounds = 10;
		m_pRoundKeys = new DWORD[4 * (m_nRounds + 1)];
		m_dir = dir;
		if(edEncrypt == m_dir)
		{
			rijndaelKeySetupEnc((u32*)m_pRoundKeys, (const u8 *)key, 128);
		}
		else
		{
			rijndaelKeySetupDec((u32*)m_pRoundKeys, (const u8 *)key, 128);
		}
		m_bOK = true;
	}
	else if(keyLen == KL192)
	{
		//support later
		m_bOK = false;
	}
	else if(keyLen == KL256)
	{
		//support later
		m_bOK = false;
	}
	else
	{
		m_bOK = false;
	}
}


void CAes::Init(const wchar_t *key, CAes::aesDirection dir, CAes::aesKeyLen keyLen)
{
	char		keyA[64];
	::WideCharToMultiByte(CP_ACP, 0, key, -1, keyA, 64, NULL, NULL);
	Init(keyA, dir, keyLen);
}


bool CAes::BurnBlock(AES_BLOCK *input, AES_BLOCK *output, DWORD nBlocks)
{
	if(!m_bOK) return false;

	if(!input || !output || 0 == nBlocks) return false;

	if(edEncrypt == m_dir)
	{
		while(nBlocks)
		{
			rijndaelEncrypt((u32 *)m_pRoundKeys, m_nRounds, (const u8 *)input, (u8 *)output);
			nBlocks--, input++, output++;
		}
	}
	else if(edDecrypt == m_dir)
	{
		while(nBlocks)
		{
			rijndaelDecrypt((u32 *)m_pRoundKeys, m_nRounds, (const u8 *)input, (u8 *)output);
			nBlocks--, input++, output++;
		}
	}
	else
	{
		//error!
		//never run here
		return false;
	}
	return true;
}

DWORD CAes::GetMinMultiAesBlock(DWORD len)
{
	DWORD	nBlock = sizeof(AES_BLOCK);
	DWORD	dwMinBlock = 0;

	if(0 == len) return 0;

	if(len <= nBlock)
	{
		dwMinBlock = nBlock;
	}
	else
	{
		dwMinBlock = len / nBlock;
		if(len % nBlock) dwMinBlock++;
		dwMinBlock *= nBlock;
	}
	return dwMinBlock;
}