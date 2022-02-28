// LSCrypt.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "LSCrypt.h"
#include "LSFileCrypt.h"
#include "scrypt.h"
#include <time.h>
#include "MainDlg.h"
#include "passwordDlg.h"
#include "ProgressDlg.h"

//全局变量
HINSTANCE	g_hInst = NULL;
UINT ExeProc(LPVOID param);


class CLSCryptModule : public CAtlDllModuleT< CLSCryptModule >
{
public :
	DECLARE_LIBID(LIBID_LSCryptLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_UFCRYPT, "{1F0E8D55-069C-4EAC-927F-408E1D194D76}")
};

CLSCryptModule _AtlModule;


#ifdef _MANAGED
#pragma managed(push, off)
#endif

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hInst = hInstance;
		
		//registry common controls
		//InitCommonControlsEx(NULL);	
		break;
	case DLL_THREAD_ATTACH:
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_DETACH:
		break;
	}

    return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif




// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
	// If we're on NT, add ourselves to the list of approved shell extensions.

    // Note that you should *NEVER* use the overload of CRegKey::SetValue with
    // 4 parameters.  It lets you set a value in one call, without having to 
    // call CRegKey::Open() first.  However, that version of SetValue() has a
    // bug in that it requests KEY_ALL_ACCESS to the key.  That will fail if the
    // user is not an administrator.  (The code should request KEY_WRITE, which
    // is all that's necessary.)

    if ( 0 == (GetVersion() & 0x80000000UL) )
        {
        CRegKey reg;
        LONG    lRet;

        lRet = reg.Open ( HKEY_LOCAL_MACHINE,
                          _T("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
                          KEY_SET_VALUE );

        if ( ERROR_SUCCESS != lRet )
            return E_ACCESSDENIED;

        lRet = reg.SetValue ( _T("LSCrypt extension"), 
                              _T("{7EC92FD3-608B-4DD9-96BB-E8732DC7771B}") );

        if ( ERROR_SUCCESS != lRet )
            return E_ACCESSDENIED;
        }

    // registers object, typelib and all interfaces in typelib
    return _AtlModule.RegisterServer(FALSE);

}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	
    // If we're on NT, remove ourselves from the list of approved shell extensions.
    // Note that if we get an error along the way, I don't bail out since I want
    // to do the normal ATL unregistration stuff too.

    if ( 0 == (GetVersion() & 0x80000000UL) )
        {
        CRegKey reg;
        LONG    lRet;

        lRet = reg.Open ( HKEY_LOCAL_MACHINE,
                          _T("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
                          KEY_SET_VALUE );

        if ( ERROR_SUCCESS == lRet )
            {
            lRet = reg.DeleteValue ( _T("{7EC92FD3-608B-4DD9-96BB-E8732DC7771B}") );
            }
        }

    return _AtlModule.UnregisterServer(FALSE);
}

void WINAPI DllClearReg(void)
{

	HKEY		hKey = NULL;
	LONG		lRet=0;
	wchar_t		*_SubKey[]=
	{
		L"*\\ShellEx\\ContextMenuHandlers",
		L"Directory\\shellex\\ContextMenuHandlers"
	};
	int count = sizeof(_SubKey)/sizeof(_SubKey[0]);


	for(int i=0; i<count; i++)
	{
		lRet = ::RegOpenKeyExW(HKEY_CLASSES_ROOT, _SubKey[i], 0, KEY_WRITE, &hKey);

		if(ERROR_SUCCESS == lRet)
		{
			::RegDeleteKey(hKey, L"LSCrypt");
			::RegCloseKey(hKey);
		}
	}
}

//判断是否注册到右键菜单
BOOL WINAPI DLLCheckeRegister(void)
{
	BOOL		bRet = FALSE;
	HKEY		hKey = NULL;
	LONG		lRet=0;
	wchar_t		*_SubKey[]=
	{
		L"*\\ShellEx\\ContextMenuHandlers\\LSCrypt",
		L"Directory\\shellex\\ContextMenuHandlers\\LSCrypt"
	};
	int count = sizeof(_SubKey)/sizeof(_SubKey[0]);


	for(int i=0; i<count; i++)
	{
		lRet = ::RegOpenKeyExW(HKEY_CLASSES_ROOT, _SubKey[i], 0, KEY_QUERY_VALUE, &hKey);

		if(ERROR_SUCCESS == lRet)
		{
			bRet = TRUE;
		}
	}
	return bRet;
}


DWORD WINAPI DllDecrypt(const char *password, const wchar_t *file, pfnEn_DeCallback pCallback, 
						LPWSTR lpAutoFileOut, int nAutoFileOut, DWORD *pLastSystemError)
{
	LSFileCrypt		ufc;
	char			pwd[64]={0};
	DWORD			nStatus = 0;
	
	if(!password) return LSC_ERR_DECRYPT_FILE_FORMAT;
	if(!file) return LSC_ERR_INPUT_FILE_NOT_FOUND;

	lstrcpynA(pwd, password,64);
	ufc.SetCallback(pCallback);
	nStatus = ufc.Decrypt(pwd, file, NULL, true, true, lpAutoFileOut, nAutoFileOut);

	if(pLastSystemError)
	{
		*pLastSystemError = ufc.GetLastSystemError();
	}

	return nStatus;
}


//encrypt a file 
DWORD WINAPI DllEncrypt(const char *password, const wchar_t *file, pfnEn_DeCallback pCallback, 
						LPWSTR lpAutoFileOut, int nAutoFileOut, DWORD *pLastSystemError)
{
	LSFileCrypt		ufc;
	char			pwd[64]={0};
	DWORD			nStatus = 0;
	
	if(!password) return LSC_ERR_DECRYPT_FILE_FORMAT;
	if(!file) return LSC_ERR_INPUT_FILE_NOT_FOUND;

	lstrcpynA(pwd, password,64);
	ufc.SetCallback(pCallback);
	nStatus = ufc.Encrypt(pwd, file, NULL, true, true, lpAutoFileOut, nAutoFileOut);

	if(pLastSystemError)
	{
		*pLastSystemError = ufc.GetLastSystemError();
	}

	return nStatus;
}


DWORD WINAPI DllOpenCrypt(const wchar_t *file)
{
	LSFileCrypt		ufc;
	char			pwd[64]={0};
	DWORD			nStatus = 0;
	//
	//if(!password) return LSC_ERR_DECRYPT_FILE_FORMAT;
	//if(!file) return LSC_ERR_INPUT_FILE_NOT_FOUND;

	//lstrcpynA(pwd, password,64);
	//ufc.SetCallback(pCallback);
	//nStatus = ufc.Decrypt(pwd, file, NULL, true, true, lpAutoFileOut, nAutoFileOut);

	//if(pLastSystemError)
	//{
	//	*pLastSystemError = ufc.GetLastSystemError();
	//}

	CProgressDlg		dlg(DIR_DECRYPT);
	CPasswordDlg	password(PASSWORD_MODE_DECRYPT);
	

	if(IDOK == password.DoModal())
	{
		dlg.SetPassword(password.GetPassword());
		lstrcpyW(dlg.m_FilePath,file);
		if(IDOK==dlg.DoModal())
		{

			SHELLEXECUTEINFOW		info={0};
			WCHAR					changefile[MAX_PATH] = {0};
			WCHAR					verb[]=L"open";

			GetShortPathNameW((LPCTSTR)dlg.m_OutFilePath, changefile, MAX_PATH);

			if(lstrlenW(changefile)>3)
			{

				//CoInitializeEx(NULL,COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
				info.cbSize = sizeof(SHELLEXECUTEINFO);
				info.fMask = SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_NO_UI;
				info.lpFile = changefile;
				info.lpVerb = verb;
				info.nShow = SW_SHOWNORMAL;

				BOOL bte=ShellExecuteEx(&info);

				if(info.hProcess)
					WaitForSingleObject(info.hProcess, INFINITE);

				CProgressDlg		dlgEn(DIR_ENCRYPT);
				dlgEn.SetPassword(password.GetPassword());
				lstrcpyW(dlgEn.m_FilePath,changefile);
				dlgEn.DoModal();
			}
		}

	}

	//m_fileList.Reset();

	return nStatus;
}

UINT ExeProc(LPVOID param)
{
	SHELLEXECUTEINFOW		info={0};
	WCHAR					file[MAX_PATH];
	WCHAR					verb[]=L"open";

	GetShortPathNameW((LPCTSTR)param, file, MAX_PATH);

	info.cbSize = sizeof(SHELLEXECUTEINFO);
	info.fMask = SEE_MASK_NOCLOSEPROCESS;
	info.lpFile = file;
	info.lpVerb = verb;
	info.nShow = SW_SHOWNORMAL;
	
	ShellExecuteEx(&info);
	
	//g_hProcess = info.hProcess;

	return 0;
}