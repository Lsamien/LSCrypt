//UFileCrypt.h
#pragma once

//#include "scrypt.h"
#include "LSFile.h"
#include <list>

using namespace std;

class LSFileCrypt
{
private:	
	LSFile				m_fIn;
	LSFile				m_fOut;
	pfnEn_DeCallback	m_pCallback;
	DWORD				m_dwLastSystemError;
private:
	DWORD DynamicBufferSize(unsigned __int64 fileSize);
	wchar_t *GetDefaultEnFileName(const wchar_t *lpFileInFileName, wchar_t *lpDefaultOutFileName);
public:
	LSFileCrypt();
	~LSFileCrypt();
	void SetCallback(pfnEn_DeCallback pCallback);
	//LPCWSTR lpAutoFileOut = NULL, 当lpFileOut为NULL时,
	//Encrypt Decrypt使用自动输出文件名, lpAutoFileOut将接收此文件名
	DWORD Encrypt(char *key, LPCWSTR lpFileIn, LPCWSTR lpFileOut, 
		bool bOutputFileExistsFailed, bool bDeleteInFile, LPWSTR lpAutoFileOut = NULL, int nAutoFileOut = 0);
	DWORD Decrypt(char *key, LPCWSTR lpFileIn, LPCWSTR lpFileOut, 
		bool bOutputFileExistsFailed, bool bDeleteInFile, LPWSTR lpAutoFileOut = NULL, int nAutoFileOut = 0);
	DWORD EncryptDirectory(char *key, LPCWSTR lpDirectory);
	DWORD DecryptDirectory(char *key, LPCWSTR lpDirectory);
	DWORD GetLastSystemError(void) { return m_dwLastSystemError; }
	static wchar_t *GetTempFile(wchar_t *file);
	static bool IsFileExists(LPCWSTR lpFilePath);
	static wchar_t *GetPath(const wchar_t *lpFullPathSrc, wchar_t *path);
	static wchar_t *GetPathFileName(const wchar_t *lpFullPathSrc, wchar_t *fileName);
	//根据lpDirectory给定的文件夹, 找出所有子文件夹和文件 , 分别保存到各个list中
	//返回文件的总的数量
	//
	static int GetDirFileListEx(const wchar_t *lpDirectory, 
		list<wchar_t *> &folderList, list<wchar_t *> &fileList, bool bIgnoreFolder, bool bIgnoreFile);
	//GetDirFileList_free负责释放GetDirFileList总分配的内存空间
	static void GetDirFileList_free(list<wchar_t *> &_list);
	//取得lpDirectory目录下所有文件的数量
	//static int GetNumberOfFile(const wchar_t *lpDirectory, bool bIncludeSubFolder);
	//取得lpDirectory目录下所有文件的相对文件路径, 保存到fileList中, 深度优先
	static int GetAllRefFileList(const wchar_t *lpDirectory, 
		const wchar_t *lpRefParentDir, //相对父目录
		list<wchar_t *> &fileList);
	static wchar_t *strcpyExW(wchar_t *d, const wchar_t *s, wchar_t **pPointToEnd, int *pLen);
	//获取文件的扩展名
	static wchar_t *GetFileExtName(const wchar_t *file, wchar_t *ext);
};


