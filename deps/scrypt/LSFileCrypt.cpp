//UFileCrypt.cpp
#include <windows.h>
#include <shlobj.h>
#include "scrypt.h"
#include "LSFileCrypt.h"

LSFileCrypt::LSFileCrypt()
{
	m_pCallback = NULL;
	m_dwLastSystemError = 0;
}

LSFileCrypt::~LSFileCrypt()
{
}

DWORD LSFileCrypt::DynamicBufferSize(unsigned __int64 fileSize)
{
	DWORD nBufferSize = 0;

	if(fileSize >= MB400)
	{
		nBufferSize = MB2;
	}
	else if(fileSize >= MB100)
	{
		nBufferSize = MB1;
	}
	else if(fileSize >= MB80)
	{
		nBufferSize = KB512;
	}
	else if(fileSize >= MB40)
	{
		nBufferSize = KB256;
	}
	else if(fileSize >= MB20)
	{
		nBufferSize = KB128;
	}
	else  if(fileSize >= MB10)
	{
		nBufferSize = KB64;
	}
	else
	{
		nBufferSize = (DWORD)fileSize;
	}

	return nBufferSize;
}




wchar_t *LSFileCrypt::GetDefaultEnFileName(const wchar_t *lpFileInFileName, wchar_t *lpDefaultOutFileName)
{
	wchar_t		fileName[MAX_PATH];
	wchar_t		*pTemp = NULL;

	if(!lpFileInFileName || !lpDefaultOutFileName) return NULL;

	GetPath(lpFileInFileName, lpDefaultOutFileName);
	GetPathFileName(lpFileInFileName, fileName);
	pTemp = fileName + lstrlenW(fileName) - 1;
	//���ļ����仯
	//�� 1.txt  --->   1_txt.LSC
	while(pTemp != fileName && *pTemp != L'.')
	{
		pTemp--;
	}
	if(*pTemp == L'.') *pTemp = L'_';
	lstrcatW(fileName, L".");
	lstrcatW(fileName, DEFAULT_EXT);

	//����ȫ·��
	lstrcatW(lpDefaultOutFileName, fileName);

	return lpDefaultOutFileName;
}

void LSFileCrypt::SetCallback(pfnEn_DeCallback pCallback)
{
	m_pCallback = pCallback;
}


DWORD LSFileCrypt::Encrypt(char *key, LPCWSTR lpFileIn, LPCWSTR lpFileOut, 
							   bool bOutputFileExistsFailed, bool bDeleteInFile,
							   LPWSTR lpAutoFileOut/* = NULL*/, int nAutoFileOut/* = 0*/)
{
	DWORD				error = LSC_ERR_SYSTEM;
	BOOL				bRet = FALSE;
	unsigned __int64	fileSize = 0;
	DWORD				fileSize1 = 0, fileSize2 = 0;
	BYTE				*pBuffer = NULL;
	int					nBufferSize = 0;
	unsigned __int64	nCurrentPos;
	DWORD				dwRet = 0;
	wchar_t				*pDefaultFileName = NULL;
	wchar_t				*pExt = NULL;		//��չ��
	DWORD				nCallbackRet = EN_DE_CALLBACK_EVENT_CONTINUE;

	OutputDebugStringW(L"Enter Encrypt\n");
	//reset system error;
	m_dwLastSystemError = 0;

	if(bOutputFileExistsFailed && IsFileExists(lpFileOut))
	{
#if defined(_DEBUG) || defined(Release_check)
		OutputDebugStringW(L"����ļ��Ѿ�����:");
		OutputDebugStringW(lpFileOut);
		OutputDebugStringW(L"\n:");
#endif
		return LSC_ERR_OUTPUT_FILE_ALREADY_EXISTS;
	}

	if(!m_fIn.Open(lpFileIn, GENERIC_READ, 0, OPEN_EXISTING))
	{
		if(IsFileExists(lpFileIn))
		{
			//�ļ�����, ��ȡ�������
#if defined(_DEBUG) || defined(Release_check)
			wchar_t buffer[512];

			m_dwLastSystemError = GetLastError();
			swprintf_s(buffer, 512, L"�������ļ� %s ʧ��, ������� %d\n", lpFileIn, m_dwLastSystemError);
			OutputDebugStringW(buffer);
#endif
			error = LSC_ERR_SYSTEM;
		}
		else
		{
#if defined(_DEBUG) || defined(Release_check)
			OutputDebugStringW(L"�����ļ�δ����:");
			OutputDebugStringW(lpFileIn);
			OutputDebugStringW(L"\n:");
#endif
			error = LSC_ERR_INPUT_FILE_NOT_FOUND;
		}
		goto END_EN;
	}
	//�ж��ļ�����չ��
	pExt = new wchar_t[MAX_PATH];
	GetFileExtName(lpFileIn, pExt);
	if(!lstrcmpiW(pExt, DEFAULT_EXT))
	{
		//�����ܵ��ļ���չ����Ĭ�ϵ���չ����ͬ, ������
#if defined(_DEBUG) || defined(Release_check)
		OutputDebugStringW(L"�����ļ���չ����Ĭ�ϵ���չ����ͬ , �ļ���:");
		OutputDebugStringW(lpFileIn);
		OutputDebugStringW(L"\n:");
#endif
		error = LSC_ERR_FILE_IS_DEFAULT_EXT;
		goto END_EN;
	}
	//�ж��Ƿ��Ǽ��ܹ����ļ�
	//if(m_fIn.CheckFlags())
	//{
	//	//�ļ��Ѿ�����
	//	error = LSC_ERR_SUCCESS;
	//	goto END_EN;
	//}

	if(!lpFileOut)
	{
		pDefaultFileName = new wchar_t[MAX_PATH];
		GetDefaultEnFileName(lpFileIn, pDefaultFileName);
		lpFileOut = pDefaultFileName;
		//�ٴ��ж�����ļ����Ƿ����
		if(bOutputFileExistsFailed && IsFileExists(lpFileOut))
		{
#if defined(_DEBUG) || defined(Release_check)
			OutputDebugStringW(L"����ļ��Ѿ�����:");
			OutputDebugStringW(lpFileOut);
			OutputDebugStringW(L"\n:");
#endif
			error = LSC_ERR_OUTPUT_FILE_ALREADY_EXISTS;
			goto END_EN;
		}
	}
	//�������·�����ⲿ������?
	if(lpAutoFileOut)
	{
		lstrcpynW(lpAutoFileOut, lpFileOut, nAutoFileOut);
	}

	m_fOut.Init(key, CAes::edEncrypt);
	if(!m_fOut.Open(lpFileOut, GENERIC_READ | GENERIC_WRITE, 0,  CREATE_ALWAYS)) goto END_EN;
	
	//get file size
	fileSize1 = m_fIn.GetSize(&fileSize2);

	fileSize = fileSize2;
	fileSize <<= 32;
	fileSize |= fileSize1;
	
	//��̬���ƻ�������С
	nBufferSize = DynamicBufferSize(fileSize);

	pBuffer = new BYTE[nBufferSize];

	if(!pBuffer) goto END_EN;

	if(!m_fOut.SeekToData()) goto END_EN;
	
	nCurrentPos=0;
	do
	{
		if(!m_fIn.Read(pBuffer, nBufferSize, &dwRet))
		{
			m_dwLastSystemError = GetLastError();
			error = LSC_ERR_SYSTEM;
			goto END_EN;
		}

		if(!m_fOut.Write(pBuffer, dwRet, &dwRet))
		{
			m_dwLastSystemError = GetLastError();
			error = LSC_ERR_SYSTEM;
			goto END_EN;
		}
		nCurrentPos += dwRet;
		if(m_pCallback)
		{
			nCallbackRet = m_pCallback(EN_DE_CALLBACK_EVENT_PROGRESS, fileSize, nCurrentPos);
		}
	}
	while(EN_DE_CALLBACK_EVENT_ABORT != nCallbackRet && nCurrentPos < fileSize);
	//�û�ȡ��?
	if(EN_DE_CALLBACK_EVENT_ABORT == nCallbackRet)
	{
		OutputDebugStringW(L"task abort\n");
		error = LSC_ERR_TASK_ABORT;
		goto END_EN;
	}

	if(!m_fOut.WriteHeader(&m_fIn))
	{
#if defined(_DEBUG) || defined(Release_check)
		OutputDebugStringW(L"д���ļ�ͷʧ��, �ļ�:");
		OutputDebugStringW(lpFileOut);
		OutputDebugStringW(L"\n:");
#endif
		m_dwLastSystemError = GetLastError();
		error = LSC_ERR_SYSTEM;

		goto END_EN;
	}

	error = LSC_ERR_SUCCESS;
	
END_EN:
	m_fIn.Close();
	m_fOut.Close();

	//����ȡ��, ɾ��������ļ�
	if(LSC_ERR_TASK_ABORT == error) ::DeleteFile(lpFileOut);

	delete [] pExt;
	pExt = NULL;

	delete [] pBuffer;
	pBuffer = NULL;

	delete [] pDefaultFileName;
	pDefaultFileName = NULL;

//bug	2007-11-6
	/*
	//delete old file
	if(LSC_ERR_SUCCESS == error && bDeleteInFile) ::DeleteFile(lpFileIn);
	*/
//bug
	//fixed
	//delete old file
	if(LSC_ERR_SUCCESS == error && bDeleteInFile)
	{
		//ȥ��ֻ������
		dwRet = GetFileAttributes(lpFileIn);
		dwRet = dwRet & (~FILE_ATTRIBUTE_READONLY);
		//OutputDebugStringA(msg);
		SetFileAttributes(lpFileIn,  dwRet);
		::DeleteFile(lpFileIn);
	}
	//fixed
	
	OutputDebugStringW(L"Exit Encrypt\n");
	return error;
}


DWORD LSFileCrypt::Decrypt(char *key, LPCWSTR lpFileIn, LPCWSTR lpFileOut, 
							  bool bOutputFileExistsFailed, bool bDeleteInFile,
							  LPWSTR lpAutoFileOut/* = NULL*/, int nAutoFileOut/* = 0*/)
{
	DWORD		error = LSC_ERR_SYSTEM;
	BOOL		bRet = FALSE;
	unsigned __int64	fileSize = 0;
	DWORD		fileSize1 = 0, fileSize2 = 0;
	BYTE		*pBuffer = NULL;
	int			nBufferSize = 0;
	unsigned __int64	nCurrentPos;
	DWORD		dwRet = 0;
	bool		bAllDone = false;
	wchar_t		*fOut = NULL;
	wchar_t		*pExt = NULL;		//��չ��
	wchar_t		*pOriginalFileName = NULL;
	DWORD		nCallbackRet = EN_DE_CALLBACK_EVENT_CONTINUE;

	//reset system error;
	m_dwLastSystemError = 0;

	if(bOutputFileExistsFailed && IsFileExists(lpFileOut))
	{
#if defined(_DEBUG) || defined(Release_check)
		OutputDebugStringW(L"����ļ����Ѿ�����:");
		OutputDebugStringW(lpFileOut);
		OutputDebugStringW(L"\n:");
#endif
		return LSC_ERR_OUTPUT_FILE_ALREADY_EXISTS;
	}


	m_fIn.Init(key, CAes::edDecrypt);
	if(!m_fIn.Open(lpFileIn, GENERIC_READ, 0, OPEN_EXISTING))
	{
		if(IsFileExists(lpFileIn))
		{
#if defined(_DEBUG) || defined(Release_check)
			wchar_t buffer[512];

			m_dwLastSystemError = GetLastError();
			swprintf_s(buffer, 512, L"�������ļ� %s ʧ��, ������� %d\n", lpFileIn, m_dwLastSystemError);
			OutputDebugStringW(buffer);
#endif
			error = LSC_ERR_SYSTEM;
		}
		else
		{
#if defined(_DEBUG) || defined(Release_check)
			OutputDebugStringW(L"�����ļ�δ����:");
			OutputDebugStringW(lpFileIn);
			OutputDebugStringW(L"\n:");
#endif
			error = LSC_ERR_INPUT_FILE_NOT_FOUND;
		}

		goto END_DE;
	}
	//�ж��ļ�����չ��
	pExt = new wchar_t[MAX_PATH];
	GetFileExtName(lpFileIn, pExt);
	if(lstrcmpiW(pExt, DEFAULT_EXT))
	{
#if defined(_DEBUG) || defined(Release_check)
		OutputDebugStringW(L"�����ļ���չ������Ĭ�ϵ���չ��, �ļ���:");
		OutputDebugStringW(lpFileIn);
		OutputDebugStringW(L"\n:");
#endif
		//�����ܵ��ļ���չ������Ĭ�ϵ���չ��, ������
		error = LSC_ERR_FILE_IS_NOT_DEFAULT_EXT;
		goto END_DE;
	}
	//�ж��Ƿ��Ǽ��ܹ����ļ�
	if(!m_fIn.CheckFlags())
	{
		//�޷�ʶ����ļ���ʽ
		//�����ļ���ʽ����, ͨ���ǽ���ʱ, ������ļ���δ���ܹ��Ļ��������
#if defined(_DEBUG) || defined(Release_check)
		OutputDebugStringW(L"�����ļ����ļ�ͷ�޷�ʶ��:");
		OutputDebugStringW(lpFileIn);
		OutputDebugStringW(L"\n:");
#endif
		error = LSC_ERR_DECRYPT_FILE_FORMAT;
		goto END_DE;
	}
	if(!lpFileOut)
	{
		//����ļ�ΪNULL, ��ʾʹ���ļ�������ǰ��ԭʼ�ļ�����·��
		fOut = new wchar_t[MAX_PATH];
		pOriginalFileName = new wchar_t[MAX_PATH];
		
		GetPath(lpFileIn, fOut);
		m_fIn.GetOriginalFileNameW(pOriginalFileName, MAX_PATH);
		lstrcat(fOut, pOriginalFileName);

		delete [] pOriginalFileName;
		pOriginalFileName = NULL;

		lpFileOut = fOut;
		//�ٴ��ж�����ļ��Ƿ����
		if(bOutputFileExistsFailed && IsFileExists(lpFileOut))
		{
#if defined(_DEBUG) || defined(Release_check)
			OutputDebugStringW(L"����ļ����Ѿ�����:");
			OutputDebugStringW(lpFileOut);
			OutputDebugStringW(L"\n:");
#endif
			error = LSC_ERR_OUTPUT_FILE_ALREADY_EXISTS;
			goto END_DE;
		}
	}

	//�������·�����ⲿ������?
	if(lpAutoFileOut)
	{
		lstrcpynW(lpAutoFileOut, lpFileOut, nAutoFileOut);
	}
	if(!m_fOut.Open(lpFileOut, GENERIC_READ | GENERIC_WRITE, 0,  CREATE_ALWAYS)) goto END_DE;
	
	//get file size
	fileSize1 = m_fIn.GetSize(&fileSize2);

	fileSize = fileSize2;
	fileSize <<= 32;
	fileSize |= fileSize1;

	//��ȥ�ļ�ͷ�Ŀ���
	fileSize -= m_fOut.GetHeaderSize();

	//��̬���ƻ�������С
	nBufferSize = DynamicBufferSize(fileSize);

	pBuffer = new BYTE[nBufferSize];

	if(!pBuffer) goto END_DE;
	
	nCurrentPos=0;
	if(!m_fIn.SeekToData()) goto END_DE;
	do
	{
		if(!m_fIn.Read(pBuffer, nBufferSize, &dwRet))
		{
			error = LSC_ERR_SYSTEM;
			m_dwLastSystemError = GetLastError();
			goto END_DE;
		}

		if(!m_fOut.Write(pBuffer, dwRet, &dwRet))
		{
			error = LSC_ERR_SYSTEM;
			m_dwLastSystemError = GetLastError();
			goto END_DE;
		}
		nCurrentPos += dwRet;
		if(m_pCallback)
		{
			nCallbackRet = m_pCallback(EN_DE_CALLBACK_EVENT_PROGRESS, fileSize, nCurrentPos);
		}
	}
	while(EN_DE_CALLBACK_EVENT_ABORT != nCallbackRet && nCurrentPos < fileSize);
	//�û�ȡ��?
	if(EN_DE_CALLBACK_EVENT_ABORT == nCallbackRet)
	{
		OutputDebugStringW(L"task abort\n");
		error = LSC_ERR_TASK_ABORT;
		goto END_DE;
	}
	//�۳��������
	//�����ļ�������־
	m_fOut.Seek(-m_fIn.GetBytesOfPadding(), NULL, FILE_END);
	m_fOut.SetEnd();
	error = LSC_ERR_SUCCESS;

END_DE:

	m_fIn.Close();
	m_fOut.Close();

	//����ȡ��, ɾ��������ļ�
	if(LSC_ERR_TASK_ABORT == error) ::DeleteFile(lpFileOut);


	delete [] pBuffer;
	pBuffer = NULL;

	delete [] fOut;
	fOut = NULL;

	delete [] pExt;
	pExt = NULL;
	
//bug	2007-11-12
	//delete old file
	/*
	if(LSC_ERR_SUCCESS == error && bDeleteInFile) ::DeleteFile(lpFileIn);
	*/
//bug

	//fixed
	//delete old file
	if(LSC_ERR_SUCCESS == error && bDeleteInFile)
	{
		//ȥ��ֻ������
		dwRet = GetFileAttributes(lpFileIn);
		dwRet = dwRet & (~FILE_ATTRIBUTE_READONLY);
		//OutputDebugStringA(msg);
		SetFileAttributes(lpFileIn,  dwRet);
		::DeleteFile(lpFileIn);
	}
	//fixed

	return error;
}




DWORD LSFileCrypt::EncryptDirectory(char *key, LPCWSTR lpDirectory)
{
	list<wchar_t *>		folderList;
	list<wchar_t *>		fileList;
	int					len=0;
	wchar_t				path[MAX_PATH];
	wchar_t				*pPointer = NULL;
	const wchar_t		*pDir = NULL;

	if(!lpDirectory) return LSC_ERR_SUCCESS;

	strcpyExW(path, lpDirectory, &pPointer, &len);
	if(path[len - 1] != L'\\')
	{
		path[len] = L'\\';
		path[len + 1] = 0;
		pPointer++;
		len++;
	}
	
	//pDir = lpDirectory;
	//pPointer = path;

	////copy lpDirectory to path
	//while(*pDir)
	//{
	//	*pPointer = *pDir;
	//	pPointer++;
	//	pDir++;
	//	len++;
	//}
	//if(path[len - 1] != L'\\')
	//{
	//	path[len] = L'\\';
	//	pPointer++;
	//	len++;
	//}
	//path[len] = 0;

	GetDirFileListEx(lpDirectory, folderList, fileList, false, false);

	list<wchar_t *>::iterator inter;

	//encrypt file
	for(inter = fileList.begin(); inter != fileList.end(); inter++)
	{
		lstrcpyW(pPointer, *inter);
		//�������Encrypt�ķ���ֵ, ������Encrypt���洦��
		Encrypt(key, path, NULL, true, true);
	}
	//�ݹ�Ŀ¼
	for(inter = folderList.begin(); inter != folderList.end(); inter++)
	{
		lstrcpyW(pPointer, *inter);
		//�ݹ�
		EncryptDirectory(key, path);
	}
	//clean up
	GetDirFileList_free(folderList);
	GetDirFileList_free(fileList);

	return LSC_ERR_SUCCESS;
}


DWORD LSFileCrypt::DecryptDirectory(char *key, LPCWSTR lpDirectory)
{
	list<wchar_t *>		folderList;
	list<wchar_t *>		fileList;
	int					len=0;
	wchar_t				path[MAX_PATH];
	wchar_t				*pPointer = NULL;
	const wchar_t		*pDir = NULL;

	if(!lpDirectory) return LSC_ERR_SUCCESS;

	strcpyExW(path, lpDirectory, &pPointer, &len);
	if(path[len - 1] != L'\\')
	{
		path[len] = L'\\';
		path[len + 1] = 0;
		pPointer++;
		len++;
	}
	
	//pDir = lpDirectory;
	//pPointer = path;

	////copy lpDirectory to path
	//while(*pDir)
	//{
	//	*pPointer = *pDir;
	//	pPointer++;
	//	pDir++;
	//	len++;
	//}
	//if(path[len - 1] != L'\\')
	//{
	//	path[len] = L'\\';
	//	pPointer++;
	//	len++;
	//}
	//path[len] = 0;

	GetDirFileListEx(lpDirectory, folderList, fileList, false, false);

	list<wchar_t *>::iterator inter;

	//encrypt file
	for(inter = fileList.begin(); inter != fileList.end(); inter++)
	{
		lstrcpyW(pPointer, *inter);
		//�������Encrypt�ķ���ֵ, ������Encrypt���洦��
		Decrypt(key, path, NULL, true, true);
	}
	//�ݹ�Ŀ¼
	for(inter = folderList.begin(); inter != folderList.end(); inter++)
	{
		lstrcpyW(pPointer, *inter);
		//�ݹ�
		DecryptDirectory(key, path);
	}
	//clean up
	GetDirFileList_free(folderList);
	GetDirFileList_free(fileList);

	return LSC_ERR_SUCCESS;
}

wchar_t * LSFileCrypt::GetTempFile(wchar_t *file)
{
	wchar_t		temp[MAX_PATH];
	SHGetSpecialFolderPathW(NULL, temp, CSIDL_TEMPLATES, TRUE);
	lstrcatW(temp, L"\\");

	if(GetTempFileName(temp, L"LSC", 5, file))
	{
		return file;
	}
	return NULL;
}


bool LSFileCrypt::IsFileExists(LPCWSTR lpFilePath)
{
	DWORD		dwAttributes = INVALID_FILE_ATTRIBUTES;

	dwAttributes = GetFileAttributesW(lpFilePath);

	if(dwAttributes != INVALID_FILE_ATTRIBUTES && 
		!(dwAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		return true;
	}

	return false;
}


wchar_t *LSFileCrypt::GetPath(const wchar_t *lpFullPathSrc, wchar_t *path)
{
	//lpFullPathSrc��ȫ·���ļ���, �ú���ȡ��lpFullPathSrc��·��, �������ļ���,����·����path
	const wchar_t *pSrc = NULL;
	wchar_t *pDes = NULL;
	int		len;

	if(!lpFullPathSrc || !path) return NULL;

	pSrc = lpFullPathSrc;
	pDes = path;
	
	len = 0;
	while(*pSrc)
	{
		*pDes = *pSrc;
		pDes++;
		pSrc++;
		len++;
	}
	path[len] = 0;
	pDes = path + len;

	while(*pDes != L'\\' && pDes != path)
	{
		*pDes = 0;
		pDes--;
	}

	return path;
}

wchar_t *LSFileCrypt::GetPathFileName(const wchar_t *lpFullPathSrc, wchar_t *fileName)
{
	const wchar_t	*pFile = NULL;
	int		len = 0;

	if(!lpFullPathSrc || !fileName) return NULL;

	len = lstrlenW(lpFullPathSrc);
	pFile = lpFullPathSrc + len;
	//lstrcpynA(file, m_header.fileName, nBuffer);
	while(*pFile != L'\\' && pFile != lpFullPathSrc)
	{
		pFile--;
	}
	pFile++;
	while(*pFile)
	{
		*fileName = *pFile;
		fileName++;
		pFile++;
	}
	*fileName = 0;
	return fileName;
}


 int LSFileCrypt::GetDirFileListEx(const wchar_t *lpDirectory, 
		list<wchar_t *> &folderList, list<wchar_t *> &fileList, 
		bool bIgnoreFolder, bool bIgnoreFile)
 {
	WIN32_FIND_DATAW	wfd={0};
	HANDLE				hFind = INVALID_HANDLE_VALUE;
	wchar_t				path[MAX_PATH];
	BOOL				bMore = FALSE;
	wchar_t				*pTemp = NULL;
	int					len=0;
	wchar_t				*pMidPointer = NULL;
	int					count = 0;	

	strcpyExW(path, lpDirectory, &pMidPointer, &len);
	if(path[len - 1] != L'\\')
	{
		path[len] = L'\\';
		path[len + 1] = 0;
		pMidPointer++;
		len++;
	}

	//lstrcpynW(path, lpDirectory, MAX_PATH);
	//len = lstrlenW(lpDirectory);
	//if(path[len - 1] != L'\\')
	//{
	//	path[len] = L'\\';
	//	path[len + 1] = 0;
	//	len++;
	//}
	//pMidPointer = path + len;

	lstrcat(path, L"*.*");

	hFind = ::FindFirstFile(path, &wfd);

	if(INVALID_HANDLE_VALUE != hFind)
	{
		bMore = ::FindNextFileW(hFind, &wfd);
		while(bMore)
		{
			if(!lstrcmp(wfd.cFileName, L".") || !lstrcmp(wfd.cFileName, L".."))
			{
				bMore = ::FindNextFileW(hFind, &wfd);
				continue;
			}

			if(!bIgnoreFolder || !bIgnoreFile)
			{
				len = lstrlenW(wfd.cFileName);
				pTemp = new wchar_t[len + 1];
				lstrcpynW(pTemp, wfd.cFileName, len + 1);
				if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(bIgnoreFolder)
					{
						delete [] pTemp;
					}
					else
					{
						//directory, add to list
						folderList.push_back(pTemp);
					}
					
				}
				else
				{
					if(bIgnoreFile)
					{
						delete [] pTemp;
					}
					else
					{
						//file, encrypt it
						fileList.push_back(pTemp);
						count++;
					}
				}
			}
			pTemp = NULL;
			bMore = ::FindNextFileW(hFind, &wfd);			
		}
		::FindClose(hFind);
	}

	return count;
 }


 void LSFileCrypt::GetDirFileList_free(list<wchar_t *> &_list)
 {
	 list<wchar_t *>::iterator inter;

	 //clean up
	 for(inter = _list.begin(); inter != _list.end(); inter++)
	 {
		 delete [] (wchar_t *)*inter;
	 }
	 _list.clear();
 }

/*
int LSFileCrypt::GetNumberOfFile(const wchar_t *lpDirectory, bool bIncludeSubFolder)
 {
	int					count = 0;
	list<wchar_t *>		folderList;
	list<wchar_t *>		fileList;
	list<wchar_t *>::iterator		iter;
	wchar_t				*pSubDirectory = NULL;
	wchar_t				*p = NULL;
	int					len;

	if(!lpDirectory) return 0;

	GetDirFileListEx(lpDirectory, folderList, fileList, !bIncludeSubFolder, false);
	//count file
	for(iter = fileList.begin(); iter != fileList.end(); iter++)
	{
		count++;
	}	

	if(bIncludeSubFolder)
	{
		for(iter = folderList.begin(); iter != folderList.end(); iter++)
		{
			if(!pSubDirectory)
			{
				pSubDirectory = new wchar_t[MAX_PATH];
				strcpyExW(pSubDirectory, lpDirectory, &p, &len);
				if(pSubDirectory[len - 1] != L'\\')
				{
					pSubDirectory[len] = L'\\';
					pSubDirectory[len+1] = 0;
					len += 1;
					p++;
				}
			}
			lstrcpyW(p, *iter);
			count += GetNumberOfFile(pSubDirectory, bIncludeSubFolder);
		}
		delete [] pSubDirectory;
		pSubDirectory = NULL;
	}
	
	GetDirFileList_free(folderList);
	GetDirFileList_free(fileList);

	return count;
 }
*/



int LSFileCrypt::GetAllRefFileList(const wchar_t *lpDirectory, 
								const wchar_t *lpRefParentDir, 
								list<wchar_t *> &fileList)
{
	WIN32_FIND_DATAW	wfd={0};
	HANDLE				hFind = INVALID_HANDLE_VALUE;
	wchar_t				path[MAX_PATH];
	BOOL				bMore = FALSE;
	wchar_t				*pTemp = NULL;
	int					len=0;
	//wchar_t				*pPointer = NULL;
	int					count = 0;	
	int					nLenParentDir = 0;
	list<wchar_t *>		folderList;
	list<wchar_t *>::iterator	iter;

	if(!lpDirectory) return 0;

	//strcpyExW(path, lpDirectory, &pPointer, &len);
	//if(path[len - 1] != L'\\')
	//{
	//	path[len] = L'\\';
	//	path[len + 1] = 0;
	//	pPointer++;
	//	len++;
	//}
	lstrcpyW(path, lpDirectory);
	lstrcatW(path, L"\\");
	lstrcatW(path, lpRefParentDir);
	lstrcatW(path, L"\\");
	lstrcatW(path, L"*.*");

	nLenParentDir = lstrlenW(lpRefParentDir);

	hFind = ::FindFirstFile(path, &wfd);

	if(INVALID_HANDLE_VALUE != hFind)
	{
		bMore = ::FindNextFileW(hFind, &wfd);
		while(bMore)
		{
			if(!lstrcmp(wfd.cFileName, L".") || !lstrcmp(wfd.cFileName, L".."))
			{
				bMore = ::FindNextFileW(hFind, &wfd);
				continue;
			}
			len = lstrlenW(wfd.cFileName);
			pTemp = new wchar_t[nLenParentDir + len + 3];		// '\0' '\\' '\\'
			pTemp[0] = 0;
			if(lpRefParentDir)
			{
				lstrcpyW(pTemp, lpRefParentDir);
				lstrcatW(pTemp, L"\\");
			}	
			lstrcatW(pTemp, wfd.cFileName);

			if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//directory, add to list
				folderList.push_back(pTemp);					
			}
			else
			{
				//file, encrypt it
				fileList.push_back(pTemp);
				count++;
			}
			pTemp = NULL;
			bMore = ::FindNextFileW(hFind, &wfd);						
		}
		::FindClose(hFind);
	}
	//*pPointer = 0;		//�ض�path, ��path��Ȼ== lpDirectory
	for(iter = folderList.begin(); iter != folderList.end(); iter++)
	{
		//lstrcatW(pPointer, *iter);
		//�ݹ�
		count += GetAllRefFileList(lpDirectory, *iter, fileList);
	}
	return count;
}


wchar_t *LSFileCrypt::strcpyExW(wchar_t *d, const wchar_t *s, wchar_t **pPointToEnd, int *pLen)
{
	//��s������d
	//��pPointToEndָ��d�ַ�����β��, ��0
	//���ַ����ĳ���д��pLen
	int		len = 0;
	wchar_t	*p = NULL;

	p = d;
	if(s && p)
	{
		while(*s)
		{
			*p = *s;
			p++;
			s++;
			len++;
		}
		*p = L'\0';
		if(pPointToEnd) *pPointToEnd = p;
	}
	else
	{
		if(pPointToEnd) *pPointToEnd = NULL;
	}
	if(pLen) *pLen = len;

	return d;
}


wchar_t *LSFileCrypt::GetFileExtName(const wchar_t *file, wchar_t *ext)
{
	int				len;
	const wchar_t	*pFile = file;
	wchar_t			*pExt = ext;

	if(!file || !ext) return NULL;

	len = lstrlenW(file);
	pFile = file + len - 1;
	while(*pFile != L'.' && pFile != file)
	{
		pFile--;
	}
	if(*pFile == L'.')
	{
		pFile++;
		while(*pFile)
		{
			*pExt++ = *pFile++;
		}
		*pExt = 0;
	}
	else
	{
		ext[0] = 0;		//����չ��
	}
	return ext;
}