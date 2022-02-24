//FileList.h
#pragma once

#include <windows.h>
#include <list>
#include <objidl.h>

using namespace std;

class CFileList
{
	//wchar_t		*m_pCurrentDir;		//µ±Ç°Ä¿Â¼
	IDataObject *m_pDataObject;
public:
	CFileList();
	~CFileList();
public:
	void SetDataObject(IDataObject *pDataObject);
	int GetFileList(list<wchar_t *> &folderList, list<wchar_t *> &fileList, int *pNumberOfFile);
	void GetFileListFree(list<wchar_t *> &_list);
	void Reset();
	//const wchar_t *GetCurrentPath(void) { return m_pCurrentDir; }
};