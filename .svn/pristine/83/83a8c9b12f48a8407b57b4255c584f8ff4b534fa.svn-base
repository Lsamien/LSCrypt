//UFile
#include <windows.h>
#include "LSFile.h"
#include <stdlib.h>
#include <time.h>

LSFile::LSFile()
{
	m_pDataBuffer = NULL;
	reset();
}


LSFile::~LSFile()
{
	delete [] m_pDataBuffer;
	m_pDataBuffer = NULL;
	Close();
}

void LSFile::InitHeader(void)
{
	BYTE		*pHeader = (BYTE *)&m_header;
	int			len = 0;

	srand((unsigned int)(time(NULL)));
	len = sizeof(LSFILE_HEADER);
	for(int i=0; i<len; i++)
	{
		*pHeader = (BYTE)(rand() % 256);
		pHeader++;
	}
	m_header.fileName[0]=0;
}


void LSFile::reset()
{
	m_hFile = INVALID_HANDLE_VALUE;
	if(!m_pDataBuffer)
	{
		m_pDataBuffer = new BYTE[KB8];
	}

	m_szFileName[0] = 0;
	//必须初始化为0,
	//m_key[0] = 0;	//BUG, rijndaelKeySetupEnc 采用KEY128会偏移12字节, 采用KEY192会偏移20字节
	//如果不统一初始化为0 , 会导致外部虽然输入了相同的密码, 也可能会导致解密失败
	memset(m_key, 0, sizeof(m_key));

	InitHeader();
}



bool LSFile::ReadHeader()
{
	BYTE		*header = NULL;
	DWORD		nHeaderSize = 0;
	bool		bOK = false;
	DWORD		nRead = 0;
	DWORD		nBlocks = 0;

	if(!IsOpen()) return false;

	nHeaderSize = CAes::GetMinMultiAesBlock(sizeof(LSFILE_HEADER));
	header = new BYTE[nHeaderSize];
	if(Read(header, nHeaderSize, &nRead))
	{
		memcpy(&m_header, header, sizeof(LSFILE_HEADER));
	}
	delete [] header;
	header = NULL;

	return bOK;
}


void LSFile::Init(const char *key, CAes::aesDirection dir)
{
	lstrcpyA(m_key, key);
	//m_aes.Init(key, dir);//bug, key如果在静态数据区, 在m_aes初始化时会导致密码不正确
	m_aes.Init(m_key, dir);//bug fixed
}

bool LSFile::WriteHeader(LSFile	*pFileIn)
{
	bool	bOK = false;
	DWORD	nBuffer = 0;
	DWORD	nWritten = 0;
	BYTE	*pTemp = NULL;

	if(!IsOpen()) return false;

	m_header.cbHeader = sizeof(LSFILE_HEADER);
	m_header.version = UFILE_CURRENT_VERION;
	m_header.flag[0] = 0xE8;
	m_header.flag[1] = 0x4C;
	m_header.flag[2] = 0x3B;
	m_header.flag[3] = 0x79;
	//m_header.padding=?Write(..);
	::WideCharToMultiByte(CP_ACP, 0, pFileIn->GetFileName(), -1, m_header.fileName, MAX_PATH, NULL, NULL);


	::SetFilePointer(m_hFile, 0, NULL, FILE_BEGIN);
	nBuffer = CAes::GetMinMultiAesBlock(sizeof(LSFILE_HEADER));
	pTemp = new BYTE[nBuffer];
	memset(pTemp, 0, nBuffer);
	memcpy(pTemp, &m_header, sizeof(LSFILE_HEADER));

	bOK = Write(pTemp, nBuffer, &nWritten);

	delete [] pTemp;
	pTemp = NULL;
	return bOK;
}


bool LSFile::CheckFlags()
{
	if(	m_header.cbHeader == sizeof(LSFILE_HEADER) &&
		m_header.version == UFILE_CURRENT_VERION &&
		m_header.flag[0] == 0xE8 &&
		m_header.flag[1] == 0x4C &&
		m_header.flag[2] == 0x3B &&
		m_header.flag[3] == 0x79)
	{
		return true;
	}

	return false;
}


bool LSFile::Open(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition)
{
	if(IsOpen()) return true;
	m_hFile = ::CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, 0, NULL);
	if(INVALID_HANDLE_VALUE != m_hFile)
	{
		lstrcpynW(m_szFileName, lpFileName, MAX_PATH);
		if(m_aes.GetDir() == CAes::edDecrypt)
		{
			ReadHeader();
		}
		return true;
	}

	return false;
}


bool LSFile::Close(void)
{
	bool bOK = true;
	if(m_hFile != INVALID_HANDLE_VALUE)
	{
		bOK = TRUE == ::CloseHandle(m_hFile);
	}
	reset();

	return bOK;
}


bool LSFile::IsOpen(void)
{
//#ifdef _DEBUG
//	if(m_hFile == INVALID_HANDLE_VALUE) MessageBoxW(NULL, L"文件没有打开!", L"LSFile::IsOpen", MB_ICONERROR);
//#endif
	return (m_hFile != INVALID_HANDLE_VALUE);
}


bool LSFile::Write(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten)
{	
	int			round = 0;
	int			nBlockSize = 0;
	BYTE		*pSrc = (BYTE *)lpBuffer;
	int			nBlocks = 0;
	int			i=0;
	DWORD		dwRemain = 0;
	BYTE		*temp = NULL;
	DWORD		dwWritten = 0;
	DWORD		nBytesOfWritten = 0;

	if(0 == nNumberOfBytesToWrite || !lpBuffer) return true;

	if(m_key[0])
	{
		nBlockSize = sizeof(AES_BLOCK);

		round = nNumberOfBytesToWrite/KB8;
		dwRemain = nNumberOfBytesToWrite%KB8;
		nBlocks = KB8/nBlockSize;
		for(i=0; i<round; i++)
		{
			m_aes.BurnBlock((AES_BLOCK *)pSrc, (AES_BLOCK *)m_pDataBuffer, nBlocks);
			if(!WriteFile(m_hFile, m_pDataBuffer, KB8, &dwWritten, NULL))
			{
				return false;
			}
			nBytesOfWritten += dwWritten;
			pSrc += KB8;
		}

		m_header.padding = 0;		//重要
		if(dwRemain)
		{
			nBlocks = dwRemain/nBlockSize;
			m_aes.BurnBlock((AES_BLOCK *)pSrc, (AES_BLOCK *)m_pDataBuffer, nBlocks);
			if(!WriteFile(m_hFile, m_pDataBuffer, nBlocks * nBlockSize, &dwWritten, NULL))
			{
				return false;
			}
			pSrc = pSrc + (nBlocks * nBlockSize);
			nBytesOfWritten += dwWritten;
			dwRemain = dwRemain % nBlockSize;
			if(dwRemain)
			{
				m_header.padding = (BYTE)(sizeof(AES_BLOCK) - dwRemain);		//重要
				temp = new BYTE[nBlockSize];
				memset(temp, 0, nBlockSize);
				memcpy(temp, pSrc, dwRemain);
				m_aes.BurnBlock((AES_BLOCK *)temp, (AES_BLOCK *)m_pDataBuffer, 1);
				if(!WriteFile(m_hFile, m_pDataBuffer, nBlockSize, &dwWritten, NULL))
				{
					delete [] temp;
					temp = NULL;
					return false;
				}
				nBytesOfWritten += dwWritten;

				delete [] temp;
				temp = NULL;
			}
		}
		if(lpNumberOfBytesWritten) *lpNumberOfBytesWritten = nBytesOfWritten;
	}
	else
	{
		return (TRUE == ::WriteFile(m_hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, NULL));
	}
	
	return true;
}


bool LSFile::Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead)
{
	DWORD		nBlocks = 0;
	BYTE		*lpDes = NULL;
	DWORD		nBytesOfDone = 0;
	DWORD		nRead = 0;
	int			nBlockSize = 0;
	DWORD		nRemain = 0;

	if(m_key[0])
	{
		//decrypt
		//nNumberOfBytesToRead必须是nBlocks的整倍数
		lpDes = (BYTE *)lpBuffer;
		nBlockSize = sizeof(AES_BLOCK);
		
		nRemain = nNumberOfBytesToRead;
		do
		{
			nRead = nRemain < KB8 ? nRemain : KB8;
			if(!::ReadFile(m_hFile, m_pDataBuffer, nRead, &nRead, NULL))
			{
#ifdef _DEBUG
				DWORD	err = GetLastError();
#endif				
				return false;
			}
			if(0 == nRead) break;
			nBytesOfDone += nRead;
			nRemain = nNumberOfBytesToRead - nBytesOfDone;
			nBlocks = nRead/nBlockSize;
			m_aes.BurnBlock((AES_BLOCK *)m_pDataBuffer, (AES_BLOCK *)lpDes, nBlocks);
			lpDes += nRead;
		}
		while(nRemain > 0);	
		if(lpNumberOfBytesRead) *lpNumberOfBytesRead = nBytesOfDone;
	}
	else
	{
		return (TRUE == ::ReadFile(m_hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, NULL));
	}
	
	return true;
}


DWORD LSFile::Seek(LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod)
{
	return SetFilePointer(m_hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
}


bool LSFile::SeekToData(void)
{
	DWORD		nPointer = 0;
	DWORD		nDataStart = 0;

	nDataStart = CAes::GetMinMultiAesBlock(sizeof(LSFILE_HEADER));
	nPointer = Seek(nDataStart, NULL, FILE_BEGIN);

	return (nPointer == nDataStart);
}


DWORD LSFile::GetSize(LPDWORD lpFileSizeHigh)
{
	return GetFileSize(m_hFile, lpFileSizeHigh);
}


char *LSFile::GetOriginalFileNameA(char *file, int nBuffer)
{
	char	*pFile = NULL;
	int		len = 0;

	if(m_header.fileName[0])
	{
		pFile = &m_header.fileName[0];
		len = lstrlenA(pFile);
		pFile = pFile + len - 1;
		//lstrcpynA(file, m_header.fileName, nBuffer);
		while(*pFile != '\\' && pFile != m_header.fileName)
		{
			pFile--;
		}
		pFile++;
		while(*pFile)
		{
			*file = *pFile;
			file++;
			pFile++;
		}
		*file = 0;
		return file;
	}
	return NULL;
}


wchar_t *LSFile::GetOriginalFileNameW(wchar_t *file, int nBuffer)
{
	char	temp[MAX_PATH];

	if(GetOriginalFileNameA(temp, MAX_PATH))
	{		
		::MultiByteToWideChar(CP_ACP, 0, temp, -1, file, nBuffer);
		return file;
	}
	return NULL;
}


DWORD LSFile::GetHeaderSize(void)
{
	return CAes::GetMinMultiAesBlock(sizeof(LSFILE_HEADER));
}


BYTE LSFile::GetBytesOfPadding(void)
{
	return m_header.padding;
}

bool LSFile::SetEnd(void)
{
	return (TRUE == ::SetEndOfFile(m_hFile));
}