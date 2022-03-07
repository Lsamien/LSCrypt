// UFCryptShellExt.cpp : Implementation of LSFCryptShellExt

#include "stdafx.h"
#include "LSCryptShellExt.h"
#include "scrypt.h"
#include "LSFileCrypt.h"
#include "MainDlg.h"
#include "passwordDlg.h"
#include "About.h"
//#include "RegistryDlg.h"


LSFCryptShellExt* pExt;
extern HINSTANCE	g_hInst;
extern BOOL WINAPI DllUT311DeviceExist(DWORD *error);
extern BOOL WINAPI DllUSBKeyDeviceExist(DWORD * error);
// LSFCryptShellExt

//CFileList LSFCryptShellExt::m_fileList;

HRESULT LSFCryptShellExt::Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObject, HKEY hDropID)
{

	pExt = this;

	GetModuleFileName(g_hInst,m_szDllDir,MAX_PATH);
	for (int i = lstrlen(m_szDllDir);i>0;i--)
	{
		if (m_szDllDir[i] == TEXT('\\'))
		{
			m_szDllDir[i+1] = TEXT('\0');
			break;
		}
	}

	TCHAR	szSystemLanguage[20] = {0};
	GetDefaultLanguage();

	//保存数据对象
	m_fileList.SetDataObject(pDataObject);
	return S_OK;
}

/**
*
* Function Name: RunRegister
*     //运行注册向导
*     
*     
* Parameters:
*    TCHAR* szKeyFile:  [Write parameter discription here]
*    TCHAR* szAppName:  [Write parameter discription here]
* 
* Return Type: BOOL
* Return Value: 
* 
*/

DWORD	LSFCryptShellExt::RunRegister(TCHAR* szKeyFile,TCHAR* szAppName)
{
	TCHAR * szV3Register = new TCHAR[MAX_PATH];
	TCHAR * szParameter  = new TCHAR[MAX_PATH];

	lstrcpy(szV3Register,m_szDllDir);
	lstrcat(szV3Register, _T("RegistryWizard.exe"));
	lstrcpy(szParameter, _T(" /appid:"));
	lstrcat(szParameter, szAppName);
	lstrcat(szParameter, _T(" /keypath:\""));
	lstrcat(szParameter, szKeyFile);
	lstrcat(szParameter, _T("\""));
	STARTUPINFO	si = {0};
	PROCESS_INFORMATION pi;
	if(CreateProcess(szV3Register, szParameter, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	delete [] szV3Register;
	delete [] szParameter;
	return pi.dwProcessId;
}
HRESULT LSFCryptShellExt::GetCommandString(UINT_PTR idCmd, UINT uFlags, 
										   UINT *pwReserved, LPSTR pszName, UINT cchMax)
{
	//目前我们只有两个菜单项(两个实际的项目, 2个分隔线), 如果不超过4个的数量, 表明参数无效
	//if(idCmd > 4) return E_INVALIDARG;

	//nothing	

	return E_INVALIDARG;
}


HRESULT LSFCryptShellExt::InvokeCommand(LPCMINVOKECOMMANDINFO lpInvokeCmdInfo)
{
	//高16位必须为0
	if(0 != HIWORD(lpInvokeCmdInfo->lpVerb)) return E_INVALIDARG;

	switch(LOWORD(lpInvokeCmdInfo->lpVerb))
	{
	case 0:
		//分隔线
		break;
	case 1:
		//加密
		DoEncrypt(lpInvokeCmdInfo->hwnd);
		::SetWindowPos(lpInvokeCmdInfo->hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		::SetWindowPos(lpInvokeCmdInfo->hwnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		return S_OK;
	case 2:
		//解密
		DoDecrypt(lpInvokeCmdInfo->hwnd);
		::SetWindowPos(lpInvokeCmdInfo->hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		::SetWindowPos(lpInvokeCmdInfo->hwnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		return S_OK;
	case 3:
		DoAbout(lpInvokeCmdInfo->hwnd);
		return S_OK;
	case 4:
		//分隔线
		break;
	default:
		break;

	}
	return E_INVALIDARG;
}



HRESULT LSFCryptShellExt::QueryContextMenu(HMENU hMenu, UINT nIndexMenu, 
										   UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
	//int		nMenuItemCount = 0;
	int		idCmd = idCmdFirst;
	TCHAR	szMenuEncrypt[50] = {0};
	TCHAR	szMenuDecrypt[50] = {0};
	TCHAR	szMenuAbout[50]	  = {0};


	switch(m_SystemLanguage)
	{
	case LANG_CHINESE:
		LoadString(g_hInst,IDS_MENU_ENCRYPT_CH,szMenuEncrypt,sizeof(szMenuEncrypt));
		LoadString(g_hInst,IDS_MENU_DECRYPT_CH,szMenuDecrypt,sizeof(szMenuDecrypt));
		LoadString(g_hInst,IDS_MENU_ABOUT_CH,szMenuAbout,sizeof(szMenuAbout));
		break;
	case LANG_ENGLISH:
		LoadString(g_hInst,IDS_MENU_ENCRYPT_EN,szMenuEncrypt,sizeof(szMenuEncrypt));
		LoadString(g_hInst,IDS_MENU_DECRYPT_EN,szMenuDecrypt,sizeof(szMenuDecrypt));
		LoadString(g_hInst,IDS_MENU_ABOUT_EN,szMenuAbout,sizeof(szMenuAbout));
		break;
	}
	//return added menu items count
	InsertMenu(hMenu, nIndexMenu++, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);
	InsertMenu(hMenu, nIndexMenu++, MF_STRING | MF_BYPOSITION | MF_POPUP, 
		idCmd + 1, szMenuEncrypt);

//ADD 忆捷 LOGO
	TCHAR	szDllPath[MAX_PATH] = {0};
#define		BITMAP_NAME	TEXT("EA－Key3.0.bmp")

	lstrcpy(szDllPath,m_szDllDir);
	lstrcat(szDllPath,BITMAP_NAME);
	if(!m_hBitmap)
	{
		
		if (GetFileAttributes(szDllPath) == (DWORD)INVALID_HANDLE_VALUE)
		{
			m_hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_CLOSE));
			m_hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP_OPEN));
			//m_hBitmap = (HBITMAP)LoadImage(g_hInst, _T("C:\\lslogo.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE| LR_VGACOLOR |LR_LOADTRANSPARENT);
			
		}
		else
		{
			m_hBitmap	= (HBITMAP)LoadImage(g_hInst,szDllPath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		}
		
	}
#undef		BITMAP_NAME
//End 忆捷 LOGO

	SetMenuItemBitmaps(hMenu, nIndexMenu - 1, MF_BYPOSITION, m_hBitmap, m_hBitmap);

	InsertMenu(hMenu, nIndexMenu++, MF_STRING | MF_BYPOSITION | MF_POPUP, 
		idCmd + 2, szMenuDecrypt);
	//SetMenuItemBitmaps(hMenu, nIndexMenu - 1, MF_BYPOSITION, m_hBitmap, m_hBitmap);


	//InsertMenu(hMenu, nIndexMenu++, MF_STRING | MF_BYPOSITION | MF_POPUP, 
	//	idCmd + 3, szMenuAbout);
	SetMenuItemBitmaps(hMenu, nIndexMenu - 1, MF_BYPOSITION, m_hBitmap, m_hBitmap);

	InsertMenu(hMenu, nIndexMenu++, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);

	idCmd += 4;

	return MAKE_SCODE(SEVERITY_SUCCESS, 0, (WORD)(idCmd - idCmdFirst));
}


DWORD LSFCryptShellExt::DoEncrypt(HWND hwndParent)
{
	CMainDlg		dlg(DIR_ENCRYPT, hwndParent);
	CPasswordDlg	password(PASSWORD_MODE_ENCRYPT, hwndParent);
	

	if(IDOK == password.DoModal())
	{
		dlg.SetPassword(password.GetPassword());
		dlg.SetTask(&m_fileList);
		dlg.DoModal();
	}

	m_fileList.Reset();
	return 0;
}


DWORD LSFCryptShellExt::DoDecrypt(HWND hwndParent)
{
	CMainDlg		dlg(DIR_DECRYPT, hwndParent);
	CPasswordDlg	password(PASSWORD_MODE_DECRYPT, hwndParent);
//Is run in V3 envirument;
	if (!IsRunInV3())
	{
		ShellExecute(NULL,_T("open"),_T("http://start.prayaya.com"),NULL,NULL,SW_NORMAL);
	}

	if(IDOK == password.DoModal())
	{
		dlg.SetPassword(password.GetPassword());
		dlg.SetTask(&m_fileList);
		dlg.DoModal();
	}

	m_fileList.Reset();
	return 0;
}


DWORD LSFCryptShellExt::DoAbout(HWND hwndParent)
{
//Is run in V3
	if (!IsRunInV3())
	{
		ShellExecute(NULL,_T("open"),_T("http://start.prayaya.com"),NULL,NULL,SW_NORMAL);
	}

	DialogBox(g_hInst,MAKEINTRESOURCE(IDD_ABOUT), hwndParent, AboutDialogProc);

	return 0;
}


void LSFCryptShellExt::GetKeyFilePath()
{
	TCHAR	szKeyFileName[MAX_PATH] = {0};
	GetModuleFileName(g_hInst,szKeyFileName,sizeof(szKeyFileName));
	lstrcpy(m_szKeyFilePath,szKeyFileName);
	for (int i = lstrlen((TCHAR*)m_szKeyFilePath);i>0;i--)
	{
		if (m_szKeyFilePath[i] == TEXT('\\'))
		{
			lstrcpyn(szKeyFileName,m_szKeyFilePath,i+2);
			break;
		}
	}
	lstrcat(szKeyFileName,KEY_FILE_NAME);
	lstrcpy(m_szKeyFilePath,szKeyFileName);
	WIN32_FIND_DATA		winfd;
	HANDLE				hFile;
	//BOOL				bFind;
	hFile = FindFirstFile(m_szKeyFilePath,&winfd);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		m_bFirstRun = TRUE;
		hFile = CreateFile(m_szKeyFilePath,GENERIC_READ|GENERIC_WRITE ,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
		}
	}
	else
	{
		m_bFirstRun = FALSE;
	}
}

void LSFCryptShellExt::GetDefaultLanguage()
{
	WORD    langID=PRIMARYLANGID(GetUserDefaultLangID());

	switch(langID)
	{
	case LANG_CHINESE:
		m_SystemLanguage = LANG_CHINESE;
		//lstrcpy(szLanguage,TEXT("Chinese Simplified"));
		break;
	default:
		//lstrcpy(szLanguage,TEXT("English"));
		m_SystemLanguage = LANG_ENGLISH;
	}
}

bool	LSFCryptShellExt::IsRunInV3()
{
	bool		bRet = false;
	LPTSTR		lpEnvString = _T("V3RunProgramFlag");
	TCHAR		lpBuffer[50] = {0};
	DWORD		dwRet;
	dwRet = GetEnvironmentVariable(lpEnvString,lpBuffer,sizeof(lpBuffer));
	if (dwRet != 0)
	{
		if (lstrcmp(lpBuffer,_T("-1")) == 0)
		{
			bRet = true;
		}
	}
	return true;
}

/*
old rgs 


HKCR
{
	UFCrypt.UFCryptShellExt.1 = s 'UFCryptShellExt Class'
	{
		CLSID = s '{7EC92FD3-608B-4DD9-96BB-E8732DC7771B}'
	}
	UFCrypt.UFCryptShellExt = s 'UFCryptShellExt Class'
	{
		CLSID = s '{7EC92FD3-608B-4DD9-96BB-E8732DC7771B}'
		CurVer = s 'UFCrypt.UFCryptShellExt.1'
	}
	NoRemove CLSID
	{
		ForceRemove {7EC92FD3-608B-4DD9-96BB-E8732DC7771B} = s 'UFCryptShellExt Class'
		{
			ProgID = s 'UFCrypt.UFCryptShellExt.1'
			VersionIndependentProgID = s 'UFCrypt.UFCryptShellExt'
			ForceRemove 'Programmable'
			InprocServer32 = s '%MODULE%'
			{
				val ThreadingModel = s 'Apartment'
			}
			val AppID = s '%APPID%'
			'TypeLib' = s '{525B8C59-09D1-4966-BE70-A07362E7D1B9}'
		}
	}
}

*/