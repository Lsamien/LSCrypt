//FileList.cpp
#include "stdafx.h"
#include "FileList.h"

CFileList::CFileList()
{
	m_pDataObject = NULL;
	//m_pCurrentDir = NULL;
}


CFileList::~CFileList()
{
	//delete [] m_pCurrentDir;
	//m_pCurrentDir = NULL;

	if(m_pDataObject)
	{
		m_pDataObject->Release();
		m_pDataObject = NULL;
	}
}




void CFileList::SetDataObject(IDataObject *pDataObject)
{
	if(m_pDataObject)
	{
		m_pDataObject->Release();
	}

	m_pDataObject = pDataObject;
	
	m_pDataObject->AddRef();	
}


int CFileList::GetFileList(list<wchar_t *> &folderList, list<wchar_t *> &fileList, int *pNumberOfFile)
{
	//将IDataObject中的文件信息保存到list中
	FORMATETC	fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM	stg = { TYMED_HGLOBAL };
	HDROP		hDrop;
	UINT		nFiles;
	wchar_t		*file = NULL;
	DWORD		attri = INVALID_FILE_ATTRIBUTES;
	int			count = 0;
	int			nNumberOfFile = 0;

	if(!m_pDataObject) return 0;
	if(FAILED(m_pDataObject->GetData(&fmt, &stg))) return 0;
	
	hDrop = (HDROP)GlobalLock(stg.hGlobal);
	if(!hDrop) return 0;
	
	nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	for(UINT i=0; i<nFiles; i++)
	{
		file = new wchar_t[MAX_PATH];
		//得到全路径
		DragQueryFileW(hDrop, i, file, MAX_PATH);
		attri = GetFileAttributesW(file);
		if(attri & FILE_ATTRIBUTE_DIRECTORY)
		{
			//folder
			folderList.push_back(file);
		}
		else
		{
			nNumberOfFile++;
			fileList.push_back(file);
		}
		file = NULL;
		count++;
	}
	//if(m_pCurrentDir)
	//{
	//	delete [] m_pCurrentDir;
	//	m_pCurrentDir = NULL;
	//}
	//m_pCurrentDir = new wchar_t[MAX_PATH];
	
	GlobalUnlock(stg.hGlobal);
	ReleaseStgMedium(&stg);

	if(pNumberOfFile) *pNumberOfFile = nNumberOfFile;

	return count;
}



void CFileList::GetFileListFree(list<wchar_t *> &_list)
{
	list<wchar_t *>::iterator inter;

	 //clean up
	 for(inter = _list.begin(); inter != _list.end(); inter++)
	 {
		 delete [] (wchar_t *)*inter;
	 }
	 _list.clear();
}



void CFileList::Reset()
{
//	delete [] m_pCurrentDir;
	//m_pCurrentDir = NULL;
	if(m_pDataObject)
	{
		m_pDataObject->Release();
	}
	m_pDataObject = NULL;
}