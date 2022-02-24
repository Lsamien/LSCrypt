//UFile
#pragma once

#include "scrypt.h"
#include "aes.h"

//header
typedef struct _LSFILE_HEADER
{
	WORD		cbHeader;	//size of header
	BYTE		version;	//current version = 1
	BYTE		padding;	//padding bytes
	BYTE		flag[4];	//flags of header, flag[0]=0xE8, flag[0]=0x4C, flag[0]=0x3B, flag[0]=0x79
	char		fileName[MAX_PATH];		//原文件名
}LSFILE_HEADER;

#define UFILE_CURRENT_VERION		1

//正确使用方法
/*
Init(...);
Open(...);
....
*/
class LSFile
{
private:
	HANDLE			m_hFile;
	BYTE			*m_pDataBuffer;
	wchar_t			m_szFileName[MAX_PATH];
	LSFILE_HEADER	m_header;
	CAes			m_aes;
	char			m_key[128];
public:
	LSFile();
	~LSFile();
private:
	void InitHeader(void);
	void reset();
protected:
	DWORD			m_error;
	DWORD			m_errorEx;
protected:
	bool ReadHeader(void);
public:
	void Init(const char *key, CAes::aesDirection dir);
	bool WriteHeader(LSFile	*pFileIn);
	bool CheckFlags(void);
	bool Open(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,  DWORD dwCreationDisposition);
	bool Close();
	bool IsOpen(void);
	bool Write(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten);
	bool Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead);
	DWORD Seek(LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod);
	bool SeekToData(void);
	DWORD GetSize(LPDWORD lpFileSizeHigh);
	DWORD LastError(void) { return m_error; }
	DWORD LastErrorEx(void) { return m_errorEx; }
	char* GetOriginalFileNameA(char *file, int nBuffer);
	wchar_t *GetOriginalFileNameW(wchar_t *file, int nBuffer);
	DWORD GetHeaderSize(void);
	BYTE GetBytesOfPadding(void);
	bool SetEnd(void);
	const wchar_t *GetFileName(void) { return m_szFileName; }
};



