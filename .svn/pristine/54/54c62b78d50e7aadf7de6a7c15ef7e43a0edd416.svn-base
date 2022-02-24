//MainDlg.cpp

#include "stdafx.h"
#include <wchar.h>
#include "PasswordDlg.h"
#include "LSCryptShellExt.h"


extern HINSTANCE g_hInst;
#define		BITMAP_LOGO	TEXT("EA_key3.0_Logo.bmp")

CPasswordDlg::CPasswordDlg(int nMode, HWND hwndParent)
{
	m_nMode = nMode;
	m_hWndParent = hwndParent;
	m_hWnd = NULL;
	memset(m_password, 0, sizeof(m_password));

	TCHAR	szBitmapPath[MAX_PATH] = {0};

	lstrcpy(szBitmapPath,pExt->m_szDllDir);
	lstrcat(szBitmapPath,BITMAP_LOGO);
	
	if (GetFileAttributes(szBitmapPath) == (DWORD)INVALID_HANDLE_VALUE)
	{
		m_hBitmapLogo	  = (HBITMAP)LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_LOGO));
	}
	else
	{
		m_hBitmapLogo	= (HBITMAP)LoadImage(g_hInst,szBitmapPath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	}

}

CPasswordDlg::~CPasswordDlg()
{
	if (m_hBitmapLogo)
	{
		DeleteObject(m_hBitmapLogo);
	}
}



bool CPasswordDlg::OnInitDialog()
{
	HWND		hwndControl = NULL;
	RECT		rect;
	POINT		point;
	TCHAR		szTtile[50]				= {0};
	TCHAR		szStaticPwd[50]			= {0};
	TCHAR		szStaticPwdConfirm[50]	= {0};
	TCHAR		szButnOk[50]			= {0};
	TCHAR		szButnCancel[50]		= {0};
	TCHAR		szTip[100]				= {0};
	UINT		uID = 0;

	hwndControl = GetDlgItem(m_hWnd, IDC_PASSWORD1);
	::SendMessageW(hwndControl, EM_SETLIMITTEXT, MAX_AES_PASSWORD_LEN, 0);

		switch(PRIMARYLANGID(GetUserDefaultLangID()))
		{
		case LANG_ENGLISH:
			LoadString(g_hInst,IDS_DLG_PWD_STATIC1_EN,szStaticPwd,sizeof(szStaticPwd));
			LoadString(g_hInst,IDS_DLG_PWD_STATIC2_EN,szStaticPwdConfirm,sizeof(szStaticPwdConfirm));
			LoadString(g_hInst,IDS_DLG_PWD_BUTN_OK_EN,szButnOk,sizeof(szButnOk));
			LoadString(g_hInst,IDS_DLG_PWD_BUTN_CANCEL_EN,szButnCancel,sizeof(szButnCancel));
			LoadString(g_hInst,IDS_DLG_PWD_TITLE_EN,szTtile,sizeof(szTtile));
			if (m_nMode == PASSWORD_MODE_ENCRYPT)
			{
				uID	= IDS_DLG_PWD_TIP_EN_EN;
			} 
			else if (m_nMode == PASSWORD_MODE_DECRYPT)
			{
				uID = IDS_DLG_PWD_TIP_DE_EN;
			}
			LoadString(g_hInst,uID,szTip,sizeof(szTip));
			break;
		case LANG_CHINESE:
			LoadString(g_hInst,IDS_DLG_PWD_STATIC1_CH,szStaticPwd,sizeof(szStaticPwd));
			LoadString(g_hInst,IDS_DLG_PWD_STATIC2_CH,szStaticPwdConfirm,sizeof(szStaticPwdConfirm));
			LoadString(g_hInst,IDS_DLG_PWD_BUTN_OK_CH,szButnOk,sizeof(szButnOk));
			LoadString(g_hInst,IDS_DLG_PWD_BUTN_CANCEL_CH,szButnCancel,sizeof(szButnCancel));
			LoadString(g_hInst,IDS_DLG_PWD_TITLE_CH,szTtile,sizeof(szTtile));
			if (m_nMode == PASSWORD_MODE_ENCRYPT)
			{
				uID	= IDS_DLG_PWD_TIP_EN_CH;
			} 
			else if (m_nMode == PASSWORD_MODE_DECRYPT)
			{
				uID = IDS_DLG_PWD_TIP_DE_CH;
			}
			LoadString(g_hInst,uID,szTip,sizeof(szTip));
			break;
		}
	
	SetWindowText(m_hWnd,szTtile);
	SetDlgItemText(m_hWnd,IDC_STATIC_PASSWORD,szStaticPwd);
	SetDlgItemText(m_hWnd,IDC_STATIC_PASSWORD_CONFIRM,szStaticPwdConfirm);
	SetDlgItemText(m_hWnd,IDOK,szButnOk);
	SetDlgItemText(m_hWnd,IDCANCEL,szButnCancel);
	SetDlgItemText(m_hWnd,IDC_TIP,szTip);

	if(PASSWORD_MODE_DECRYPT == m_nMode)
	{
		//调整界面布局
		GetWindowRect(hwndControl, &rect);

		//edit控件下移

		point.x = rect.left;
		point.y = rect.top + 15;
		ScreenToClient(m_hWnd, &point);

		SetWindowPos(hwndControl, NULL, point.x, point.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		hwndControl = GetDlgItem(m_hWnd, IDC_STATIC_PASSWORD);
		GetWindowRect(hwndControl, &rect);

		point.x = rect.left;
		point.y = rect.top + 15;
		ScreenToClient(m_hWnd, &point);		

		SetWindowPos(hwndControl, NULL, point.x, point.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	hwndControl = GetDlgItem(m_hWnd, IDC_PASSWORD2);
	::SendMessageW(hwndControl, EM_SETLIMITTEXT, MAX_AES_PASSWORD_LEN, 0);

	if(PASSWORD_MODE_DECRYPT == m_nMode)
	{
		//隐藏密码确认控件
		DWORD		dwStyle;
		dwStyle = ::GetWindowLongW(hwndControl, GWL_STYLE);
		dwStyle &= ~WS_VISIBLE;
		::SetWindowLongW(hwndControl, GWL_STYLE, dwStyle);

		hwndControl = GetDlgItem(m_hWnd, IDC_STATIC_PASSWORD_CONFIRM);
		dwStyle = ::GetWindowLongW(hwndControl, GWL_STYLE);
		dwStyle &= ~WS_VISIBLE;
		::SetWindowLongW(hwndControl, GWL_STYLE, dwStyle);
	}
	
	//SetFocus(GetDlgItem(m_hWnd, IDC_PASSWORD1));
	CenterWindow();

	return true;
}

INT_PTR CPasswordDlg::DoModal()
{
	if(PASSWORD_MODE_ENCRYPT != m_nMode && PASSWORD_MODE_DECRYPT != m_nMode)
	{
		//无效的构造
		return IDCANCEL;
	}
	return DialogBoxParamW(g_hInst, MAKEINTRESOURCE(IDD_PASSWORD), m_hWndParent, 
		(DLGPROC)PasswordDialogProc, (LPARAM)this);
}

void CPasswordDlg::OnCancel()
{
	::SendMessage(m_hWnd, WM_CLOSE, IDCANCEL, 0);
	//EndDialog(m_hWnd, IDCANCEL);
	//m_hWnd = NULL;
}

void CPasswordDlg::OnOK()
{
	char	password1[MAX_AES_PASSWORD_LEN + 1], password2[MAX_AES_PASSWORD_LEN + 1];

	GetDlgItemTextA(m_hWnd, IDC_PASSWORD1, password1, MAX_AES_PASSWORD_LEN + 1);
	GetDlgItemTextA(m_hWnd, IDC_PASSWORD2, password2, MAX_AES_PASSWORD_LEN + 1);

	TCHAR		szEmptyPwd[50]		= {0};
	TCHAR		szNotmatchPwd[50]	= {0};
	TCHAR		szInputPwd[50]		= {0};

	switch(PRIMARYLANGID(GetUserDefaultLangID()))
	{
	case LANG_ENGLISH:
		LoadString(g_hInst,IDS_DLG_PWD_EMPTY_PWD_EN,szEmptyPwd,sizeof(szEmptyPwd));
		LoadString(g_hInst,IDS_DLG_PWD_NOTMATCH_PWD_EN,szNotmatchPwd,sizeof(szNotmatchPwd));
		LoadString(g_hInst,IDS_DLG_PWD_INPUT_PWD_EN,szInputPwd,sizeof(szInputPwd));
		break;
	case LANG_CHINESE:
		LoadString(g_hInst,IDS_DLG_PWD_EMPTY_PWD_CH,szEmptyPwd,sizeof(szEmptyPwd));
		LoadString(g_hInst,IDS_DLG_PWD_NOTMATCH_PWD_CH,szNotmatchPwd,sizeof(szNotmatchPwd));
		LoadString(g_hInst,IDS_DLG_PWD_INPUT_PWD_CH,szInputPwd,sizeof(szInputPwd));
		break;
	}

	if(PASSWORD_MODE_ENCRYPT == m_nMode)
	{
		//encrypt
		if(!password1[0] && !password2[0])
		{
			::MessageBoxW(m_hWnd,szEmptyPwd, PROJECT_NAME_W, MB_ICONWARNING);
			SetFocus(GetDlgItem(m_hWnd, IDC_PASSWORD1));
			return;
		}

		if(lstrcmpA(password1, password2))
		{
			::MessageBoxW(m_hWnd,szNotmatchPwd, PROJECT_NAME_W, MB_ICONWARNING);
			SetFocus(GetDlgItem(m_hWnd, IDC_PASSWORD1));
			return;
		}
	}
	else
	{
		//decrypt
		if(!password1[0])
		{
			::MessageBoxW(m_hWnd,szInputPwd, PROJECT_NAME_W, MB_ICONWARNING);
			SetFocus(GetDlgItem(m_hWnd, IDC_PASSWORD1));
			return;
		}
	}
	lstrcpynA(m_password, password1, MAX_AES_PASSWORD_LEN + 1);
//#ifdef _DEBUG
//	::MessageBoxA(m_hWnd, m_password, PROJECT_NAME_A, MB_ICONINFORMATION);
//#endif
	//EndDialog(m_hWnd, IDOK);
	//m_hWnd = NULL;	
	::SendMessage(m_hWnd, WM_CLOSE, IDOK, 0);
}



bool CPasswordDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	WORD		lw = LOWORD(wParam);

	if(IDCANCEL == lw)
	{
		OnCancel();
		return true;
	}
	else if(IDOK == lw)
	{
		OnOK();
		return true;
	}
	return false;
}



void CPasswordDlg::CenterWindow()
{
	int			cx, cy;
	RECT		rect;

	cx = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CYSCREEN);
	
	GetWindowRect(m_hWnd, &rect);
	SetWindowPos(m_hWnd, HWND_TOP, (cx - (rect.right - rect.left))/2, (cy - (rect.bottom - rect.top))/2, 
		0, 0, SWP_NOSIZE);
}

void CPasswordDlg::OnDrawItem(WPARAM wParam,LPARAM lParam)
{
	int					nIDCtl				= (int)wParam;
	LPDRAWITEMSTRUCT	lpDrawItemStruct	= (LPDRAWITEMSTRUCT)lParam; 

	HDC		hStaticDC;
	HDC		hMemDC;
	RECT	rectStatic;
	HWND	hWnd;

	BITMAP	BMPINFO;

	if (nIDCtl == IDC_LOGO)
	{
		hStaticDC	=lpDrawItemStruct->hDC;
		hWnd		= GetDlgItem(m_hWnd,IDC_LOGO);
		GetClientRect(hWnd,&rectStatic);

		GetObject(m_hBitmapLogo,sizeof(BMPINFO),&BMPINFO);
		hMemDC	= CreateCompatibleDC(hStaticDC);
		HBITMAP	hOldBitmap = (HBITMAP)SelectObject(hMemDC,m_hBitmapLogo);
		StretchBlt(hStaticDC,rectStatic.left,rectStatic.top,
			rectStatic.right-rectStatic.left,
			rectStatic.bottom-rectStatic.top,hMemDC,
			0,0,BMPINFO.bmWidth,BMPINFO.bmHeight,SRCCOPY);
		SelectObject(hMemDC,hOldBitmap);
	}


}


bool CPasswordDlg::PreTranslateMessage(MSG* pMsg)
{
	WORD lw = LOWORD(pMsg->wParam);

	switch(pMsg->message)
	{
	case WM_INITDIALOG:
		m_hWnd = pMsg->hwnd;
		OnInitDialog();
		return true;
	case WM_COMMAND:
		if(OnCommand(pMsg->wParam, pMsg->lParam)) return true;
		break;
	case WM_DRAWITEM:
		OnDrawItem(pMsg->wParam,pMsg->lParam);
		break;
	//case WM_CLOSE:
	//	EndDialog(pMsg->hwnd, 0);
	//	m_hWnd = NULL;	
	//	return true;
	default:
		break;
	}
	return false;
}


void CPasswordDlg::SetItemText(int nIDDlgItem, const wchar_t *text)
{
	::SetDlgItemTextW(m_hWnd, nIDDlgItem, text);
}



//-----------------------------------------------------------------------------------
INT_PTR CALLBACK PasswordDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WORD		lw = LOWORD(wParam);
	static CPasswordDlg	*pDialog = NULL;
	MSG			_msg = {0};

	_msg.hwnd = hwndDlg;
	_msg.lParam = lParam;
	_msg.message = uMsg;
	_msg.wParam = wParam;
	
	if(pDialog && pDialog->m_hWnd)
	{
		if(pDialog->PreTranslateMessage(&_msg)) return 1;
	}
	switch(uMsg)
	{
	case WM_INITDIALOG:
		pDialog = (CPasswordDlg *)lParam;
		if(pDialog) 
		{
			if(pDialog->PreTranslateMessage(&_msg)) return 1;
		}
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		EndDialog(hwndDlg, wParam);
		if(pDialog)
		{
			pDialog->m_hWnd = NULL;
			pDialog = NULL;
		}
		return true;
	default:
		break;
	}
	return 0;
}
