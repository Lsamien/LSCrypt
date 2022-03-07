// UFCryptShellExt.h : Declaration of the LSFCryptShellExt

#pragma once
#include "resource.h"       // main symbols

#include "LSCrypt.h"
#include "FileList.h"
#include <comdef.h>
#include <initguid.h>
#include <shlobj.h>
//#include "Activationv3.h"


struct __declspec(uuid("000214e4-0000-0000-c000-000000000046"))
IContextMenu;

_COM_SMARTPTR_TYPEDEF(IContextMenu, __uuidof(IContextMenu));



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// LSFCryptShellExt

class ATL_NO_VTABLE LSFCryptShellExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<LSFCryptShellExt, &CLSID_LSCryptShellExt>,
	public IDispatchImpl<ILSCryptShellExt, &IID_ILSCryptShellExt, &LIBID_LSCryptLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,
	public IContextMenu
{
	//static CFileList	m_fileList;
	CFileList	m_fileList;
	HBITMAP		m_hBitmap;
	
	//wchar_t		m_pCurrentPath[MAX_PATH];
public:
	LSFCryptShellExt()
	{
		m_hBitmap = NULL;
		//m_pCurrentPath[0] = 0;
	}

	~LSFCryptShellExt()
	{
		if (m_hBitmap)
		{
			DeleteObject(m_hBitmap);
		}
	}
DECLARE_REGISTRY_RESOURCEID(IDR_LSCRYPTSHELLEXT)


BEGIN_COM_MAP(LSFCryptShellExt)
	COM_INTERFACE_ENTRY(ILSCryptShellExt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	//Init
	STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);
	//ContextMenu
	STDMETHOD(GetCommandString)(UINT_PTR idCmd, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax);
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO lpInvokeCmdInfo);
	STDMETHOD(QueryContextMenu)(HMENU hMenu, UINT nIndexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
//实现
protected:
	DWORD DoEncrypt(HWND hwndParent);
	DWORD DoDecrypt(HWND hwndParent);
	DWORD DoAbout(HWND hwndParent);

	
public:
	//注册激活部分
	//V3_FILE_KEY2W	m_V3FileKey;
	ULONG64			m_AppId;
	TCHAR			m_szKeyFilePath[MAX_PATH];
	bool			m_bRegistered;
	bool			m_bExpired;
	bool			m_bFirstRun;
	PROCESS_INFORMATION ProcessInfo;
	DWORD			m_SystemLanguage;
	TCHAR			m_szDllDir[MAX_PATH];

	DWORD RunRegister(TCHAR* szKeyFile,TCHAR* szAppName);
private:
	void			GetDefaultLanguage();
};


extern LSFCryptShellExt* pExt;
OBJECT_ENTRY_AUTO(__uuidof(LSCryptShellExt), LSFCryptShellExt)
